#include <system/RenderSystem.hpp>

#include <glad/glad.h>
#include <stdio.h>
#include <string>

#include <service/ShadowMapping.hpp>
#include <component/CSkyboxCache.hpp>

#include <engine/bundle/core/CoreBundle.hpp>
#include <engine/bundle/standalone/service/Window.hpp>

const std::vector<VertexLayout> kQuadVertices = {
    // positions        // texCoords
    {-1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f},
    {1.0f, -1.0f, 0.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f, 1.0f, 1.0f}};

const std::vector<unsigned int> kQuadIndices = {
    0, 1, 2,
    2, 3, 0};

namespace default_rendering
{
    using namespace engine;

    void RenderSystem::start()
    {
        auto renderer = services().get<Renderer>();
        auto shaderFactory = services().get<ShaderFactory>();
        auto meshFactory = services().get<MeshFactory>();
        initializeImageShader(shaderFactory, meshFactory);

        auto window = services().get<Window>();
        int width, height;
        window->getSize(width, height);

        auto shadowMapping = services().get<ShadowMapping>();
        shadowMapping->initialize();
        initializeShadowMapVisualizer(shadowMapping);

        renderer->enableMultisampling(true);

        m_whiteTexture = services().get<TextureFactory>()->WhiteTexture2D();
        m_gBuffer = renderer->allocateFramebuffer(width, height, 3, true, false);
    }

    void RenderSystem::update(float /*deltaTime*/)
    {
        const auto &[cameraEntity, cameraCache, camTransformCache, camTransform] = world().fetchAt<CCameraCache, CTransformCache, CTransform>(0);

        if (!cameraEntity)
        {
            if (!m_cameraNotFoundLogged)
            {
                Log::Print("No camera found for rendering", LogLevel::Warning);
                m_cameraNotFoundLogged = true;
            }
            return;
        }

        Renderer *renderer = services().get<Renderer>();
        int width, height;
        renderer->getViewportSize(width, height);

        glm::mat4 viewMatrix = camTransformCache->viewMatrix;
        glm::mat4 projMatrix = cameraCache->projectionMatrix;

        /// ------- Shadow Mapping -------
        auto shadowMapping = services().get<ShadowMapping>();
        glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);
        if (shadowMapping->enabled() && shadowMapping->getDepthBuffer() != 0)
        {
            auto [_, dirLight] = world().fetchAt<CDirectionalLight>(0);
            if (!dirLight)
                return;

            lightSpaceMatrix = shadowMapping->getLightSpaceMatrix(dirLight->direction, camTransform->position + camTransform->forward() * 10.0f);

            UniformCollection depthUniforms;
            depthUniforms["uLightSpaceMatrix"] = lightSpaceMatrix;

            int previousWidth, previousHeight;
            renderer->setViewport(0, 0, shadowMapping->m_width, shadowMapping->m_height, &previousWidth, &previousHeight);
            renderer->setFramebuffer(shadowMapping->getDepthBuffer());
            renderer->clear(false, true, false);

            /// ------- Render Meshes for Depth -------
            for (const auto &[entity, meshRenderer, transform] : world().fetch<CMesh, CTransformCache>())
            {
                auto &meshRef = meshRenderer->meshRef;
                depthUniforms["uModel"] = transform->modelMatrix * renderer->getLocalModel(meshRef);
                renderer->drawMesh(meshRef, shadowMapping->getDepthShader(), depthUniforms);
            }

            /// ------- Render Models for Depth -------
            for (const auto &[entity, modelRenderer, transform] : world().fetch<CModel, CTransformCache>())
            {
                modelRenderer->model.forEach([&](MeshRef modelRef, MaterialHandle & /*materialRef*/, size_t /*index*/)
                                             {
                        depthUniforms["uModel"] = transform->modelMatrix * renderer->getLocalModel(modelRef);
                        renderer->drawMesh(modelRef, shadowMapping->getDepthShader(), depthUniforms); });
            }

            // renderer->resetFramebuffer();
            renderer->revertToPreviousFramebuffer();
            renderer->setViewport(0, 0, previousWidth, previousHeight);
        }

        /// ---- Debug shadow map visualization ----
        // renderer->drawMesh(m_debugScreenQuadMesh, m_debugShadowMapShader, m_debugShadowMapUniforms);
        // return;

        // ------- Geometry Pass -------
        // renderer->setFramebuffer(m_gBuffer);
        // renderer->clear(true, true, false);
        /// ------- Render Environment -------
        if (const auto &[envEntity, environment] = world().fetchAt<CEnvironment>(0); envEntity)
        {
            if (auto [_, skyboxCache] = world().fetchFrom<CSkyboxCache>(envEntity); skyboxCache != nullptr && environment->skyboxMaterial.isValid())
            {
                environment->skyboxMaterial.uniforms["view"] = glm::mat4(glm::mat3(viewMatrix));
                environment->skyboxMaterial.uniforms["projection"] = projMatrix;
                renderer->drawMesh(skyboxCache->meshRef, environment->skyboxMaterial.shaderRef, environment->skyboxMaterial.uniforms);
            }
            else
            {
                // TODO: replace with renderer
                // services().get<Window>()->clearColor(environment->backgroundColor);
            }
        }

        /// ------- Render Meshes -------
        for (const auto &[entity, meshRenderer, transform] : world().fetch<CMesh, CTransformCache>())
        {

            setFinalRenderingUniforms(meshRenderer->material.uniforms,
                                      transform->modelMatrix * renderer->getLocalModel(meshRenderer->meshRef),
                                      viewMatrix,
                                      projMatrix,
                                      lightSpaceMatrix,
                                      shadowMapping->getShadowMap(),
                                      shadowMapping->getBias());
            renderer->drawMesh(meshRenderer->meshRef, meshRenderer->material.shaderRef, meshRenderer->material.uniforms);
        }

        /// ------- Render Models -------
        for (const auto &[entity, modelRenderer, transform] : world().fetch<CModel, CTransformCache>())
        {
            modelRenderer->model.forEach([&](MeshRef meshRef, MaterialHandle &material, size_t index)
                                         {
                if (modelRenderer->materialOverrides.size() > index) {
                    material = modelRenderer->materialOverrides[index];
                }

                setFinalRenderingUniforms(material.uniforms,
                                transform->modelMatrix * renderer->getLocalModel(meshRef),
                                viewMatrix,
                                projMatrix,
                                lightSpaceMatrix,
                                shadowMapping->getShadowMap(),
                                shadowMapping->getBias());
                renderer->drawMesh(meshRef, material.shaderRef, material.uniforms); });
        }
        // renderer->revertToPreviousFramebuffer();
    }

    void RenderSystem::setFinalRenderingUniforms(UniformCollection &uniforms, const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix, const glm::mat4 &lightSpaceMatrix, Sampler2D shadowMap, float bias)
    {
        uniforms["model"] = modelMatrix;
        uniforms["view"] = viewMatrix;
        uniforms["projection"] = projMatrix;
        uniforms["uDirLightSpaceMatrix"] = lightSpaceMatrix;
        if (shadowMap.textureRef != 0)
            uniforms["uShadowMap"] = shadowMap;
        else
            uniforms["uShadowMap"] = Sampler2D{m_whiteTexture};
        uniforms["uBias"] = bias;
    }

    void RenderSystem::initializeShadowMapVisualizer(ShadowMapping *shadowMapping)
    {
        m_debugShadowMapUniforms["uFarPlane"] = shadowMapping->getFarPlane();
        m_debugShadowMapUniforms["uNearPlane"] = shadowMapping->getNearPlane();
        m_debugShadowMapUniforms["uShadowMap"] = shadowMapping->getShadowMap();
        m_debugShadowMapUniforms["uChannels"] = glm::vec4(1, 0, 0, 0);
    }

    void RenderSystem::initializeImageShader(ShaderFactory *shaderFactory, MeshFactory *meshFactory)
    {
        m_imageShader = shaderFactory->CustomShader("__ImageShader",
                                                    "default-bundle-assets/shaders/Image.vert",
                                                    "default-bundle-assets/shaders/Image.frag", {});
        m_imageQuadMesh = meshFactory->Raw(kQuadVertices, kQuadIndices);
        Log::Print("Refs: Image Shader: " + std::to_string(m_imageShader) + ", Image Quad Mesh: " + std::to_string(m_imageQuadMesh), LogLevel::Info);
    }

} // namespace engine
