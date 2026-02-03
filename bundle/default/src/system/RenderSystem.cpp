#include <system/RenderSystem.hpp>

#include <glad/glad.h>
#include <stdio.h>
#include <string>

#include <service/ShadowMapping.hpp>

#include <component/CCamera.hpp>
#include <component/CEnvironment.hpp>
#include <component/CMeshRenderer.hpp>
#include <component/CDirectionalLight.hpp>
#include <component/CTransform.hpp>
#include <component/cache/CCameraCache.hpp>
#include <component/cache/CSkyboxCache.hpp>
#include <component/cache/CTransformCache.hpp>

#include <engine/service/platform/Window.hpp>
#include <engine/service/platform/Renderer.hpp>

const std::vector<VertexLayout> kQuadVertices = {
    // positions        // texCoords
    {-1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f},
    {1.0f, -1.0f, 0.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f, 1.0f, 1.0f}};

const std::vector<unsigned int> kQuadIndices = {
    0, 1, 2,
    2, 3, 0};

namespace default_bundle
{
    using namespace engine;

    void RenderSystem::start()
    {
        auto shadowMapping = services().get<ShadowMapping>();
        m_debugShadowMapShader = services().get<ShaderFactory>()->CustomShader("__ShadowMapVisualization",
                                                                               "default-bundle-assets/shaders/shadow_mapping/debug/shadow_map_visualization.vert",
                                                                               "default-bundle-assets/shaders/shadow_mapping/debug/shadow_map_visualization.frag", {});
        m_debugScreenQuadMesh = services().get<MeshResource>()->create(kQuadVertices, kQuadIndices);
        m_debugShadowMapUniforms["uFarPlane"] = shadowMapping->getFarPlane();
        m_debugShadowMapUniforms["uNearPlane"] = shadowMapping->getNearPlane();
        m_debugShadowMapUniforms["uShadowMap"] = shadowMapping->createDepthMap();
    }

    void RenderSystem::render(float /*deltaTime*/)
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

        glm::mat4 viewMatrix = camTransformCache->viewMatrix;
        glm::mat4 projMatrix = cameraCache->projectionMatrix;

        /// ------- Shadow Mapping -------
        // auto shadowMapping = services().get<ShadowMapping>();
        // if (shadowMapping->getDepthMap() != 0)
        // {
        //     auto [_, dirLight] = world().fetchAt<CDirectionalLight>(0);
        //     if (!dirLight)
        //         return;

        //     shadowMapping->prepareForRender(shaderResource, dirLight->direction, camTransform->position + camTransform->forward() * 10.0f);
        //     ShaderRef depthShader = shadowMapping->getDepthShader();
        //     /// ------- Render Meshes for Shadow Mapping -------
        //     for (const auto &[entity, meshRenderer, transform] : world().fetch<CMeshRenderer, CTransformCache>())
        //     {
        //         auto meshRef = meshRenderer->meshRef;

        //         shaderResource->bind(depthShader, glm::mat4(1.0f), glm::mat4(1.0f),
        //                              transform->modelMatrix *
        //                                  meshResource->getLocalModel(meshRef));

        //         meshResource->draw(meshRef);
        //     }

        //     /// ------- Render Models for Depth -------
        //     for (const auto &[entity, modelRenderer, transform] : world().fetch<CModelRenderer, CTransformCache>())
        //     {
        //         auto &modelRef = modelRenderer->modelRef;

        //         modelResource->forEach(modelRef, [&](MeshRef meshRef, MaterialRef /*materialRef*/, size_t /*index*/)
        //                                {
        //                 shaderResource->bind(depthShader, glm::mat4(1.0f), glm::mat4(1.0f),
        //                                         transform->modelMatrix * meshResource->getLocalModel(meshRef));
        //                 meshResource->draw(meshRef); });
        //     }

        //     shadowMapping->restoreAfterRender();
        // }

        /// ---- Debug shadow map visualization ----
        // shaderResource->bind(m_debugShadowMapShader, glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f));
        // shaderResource->applyUniforms(m_debugShadowMapShader, &m_debugShadowMapUniforms, *textureResource);
        // meshResource->draw(m_debugScreenQuadMesh);
        // return;

        /// ------- Render Environment -------
        if (const auto &[envEntity, environment] = world().fetchAt<CEnvironment>(0); envEntity)
        {
            if (auto [_, skyboxCache] = world().fetchFrom<CSkyboxCache>(envEntity); skyboxCache != nullptr && environment->skyboxMaterial.isValid())
            {
                // auto shaderRef = materialResource->getShaderRef(environment->skyboxMaterial);
                // auto *uniforms = materialResource->getUniforms(environment->skyboxMaterial);

                // shaderResource->bind(shaderRef, glm::mat4(glm::mat3(viewMatrix)), projMatrix, glm::mat4(1.0f));
                // shaderResource->applyUniforms(shaderRef, uniforms, *textureResource);

                // meshResource->draw(skyboxCache->meshRef);
                environment->skyboxMaterial.uniforms["view"] = glm::mat4(glm::mat3(viewMatrix));
                environment->skyboxMaterial.uniforms["projection"] = projMatrix;
                renderer->drawMesh(skyboxCache->meshRef, environment->skyboxMaterial.shaderRef, environment->skyboxMaterial.uniforms);
            }
            else
            {
                services().get<Window>()->clearColor(environment->backgroundColor);
            }
        }

        /// ------- Render Meshes -------
        for (const auto &[entity, meshRenderer, transform] : world().fetch<CMeshRenderer, CTransformCache>())
        {
            // auto meshRef = meshRenderer->meshRef;
            // auto materialRef = meshRenderer->materialRef;

            // auto *uniforms = materialResource->getUniforms(materialRef);
            // auto shaderRef = materialResource->getShaderRef(materialRef);

            // // materialResource->setUniform(materialRef, "uShadowMap", shadowMapping->getDepthMap());
            // // materialResource->setUniform(materialRef, "uDirLightSpaceMatrix", shadowMapping->getLightSpaceMatrix());
            // // materialResource->setUniform(materialRef, "uBias", shadowMapping->getBias());

            // shaderResource->bind(shaderRef, viewMatrix, projMatrix,
            //                      transform->modelMatrix * meshResource->getLocalModel(meshRef));
            // shaderResource->applyUniforms(shaderRef, uniforms, *textureResource);

            // meshResource->draw(meshRef);

            meshRenderer->material.uniforms["view"] = viewMatrix;
            meshRenderer->material.uniforms["projection"] = projMatrix;
            meshRenderer->material.uniforms["model"] = transform->modelMatrix * renderer->getLocalModel(meshRenderer->meshRef);
            renderer->drawMesh(meshRenderer->meshRef, meshRenderer->material.shaderRef, meshRenderer->material.uniforms);
        }

        /// ------- Render Models -------
        for (const auto &[entity, modelRenderer, transform] : world().fetch<CModelRenderer, CTransformCache>())
        {
            // auto &modelRef = modelRenderer->modelRef;

            // modelResource->forEach(modelRef, [&](MeshRef meshRef, MaterialRef materialRef, size_t index)
            //                        {
            // if (modelRenderer->materialOverrides.size() > index) {
            //     materialRef = modelRenderer->materialOverrides[index];
            // }

            // materialResource->setUniform(materialRef, "uShadowMap", shadowMapping->getDepthMap());
            // materialResource->setUniform(materialRef, "uDirLightSpaceMatrix", shadowMapping->getLightSpaceMatrix());
            // materialResource->setUniform(materialRef, "uBias", shadowMapping->getBias());

            // auto* uniforms  = materialResource->getUniforms(materialRef);
            // auto  shaderRef = materialResource->getShaderRef(materialRef);

            // shaderResource->bind(shaderRef, viewMatrix, projMatrix,
            //                      transform->modelMatrix * meshResource->getLocalModel(meshRef));
            // shaderResource->applyUniforms(shaderRef, uniforms, *textureResource);

            // meshResource->draw(meshRef); });
            modelRenderer->model.forEach([&](MeshRef meshRef, Material &material, size_t index)
                                         {
                if (modelRenderer->materialOverrides.size() > index) {
                    material = modelRenderer->materialOverrides[index];
                }

                material.uniforms["view"] = viewMatrix;
                material.uniforms["projection"] = projMatrix;
                material.uniforms["model"] = transform->modelMatrix * renderer->getLocalModel(meshRef);
                renderer->drawMesh(meshRef, material.shaderRef, material.uniforms); });
        }
    }

} // namespace default_bundle
