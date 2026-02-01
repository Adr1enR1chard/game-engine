#include <system/RenderSystem.hpp>

#include <glad/glad.h>
#include <stdio.h>
#include <string>

#include <component/CCamera.hpp>
#include <component/CEnvironment.hpp>
#include <component/CMeshRenderer.hpp>
#include <component/cache/CCameraCache.hpp>
#include <component/cache/CSkyboxCache.hpp>
#include <component/cache/CTransformCache.hpp>

#include <engine/service/platform/Window.hpp>
#include <engine/service/resource/MaterialResource.hpp>
#include <engine/service/resource/MeshResource.hpp>
#include <engine/service/resource/ModelResource.hpp>
#include <engine/service/resource/ShaderResource.hpp>
#include <engine/service/resource/TextureResource.hpp>

void RenderSystem::render(float /*deltaTime*/)
{
    const auto &[cameraEntity, cameraCache, cameraTransform] = world().fetchAt<CCameraCache, CTransformCache>(0);

    if (!cameraEntity)
    {
        if (!m_cameraNotFoundLogged)
        {
            Log::Print("No camera found for rendering", LogLevel::Warning);
            m_cameraNotFoundLogged = true;
        }
        return;
    }

    MeshResource *meshResource = services().get<MeshResource>();
    MaterialResource *materialResource = services().get<MaterialResource>();
    ShaderResource *shaderResource = services().get<ShaderResource>();
    ModelResource *modelResource = services().get<ModelResource>();
    TextureResource *textureResource = services().get<TextureResource>();

    glm::mat4 viewMatrix = cameraTransform->viewMatrix;
    glm::mat4 projMatrix = cameraCache->projectionMatrix;

    /// ------- Render Environment -------
    if (const auto &[envEntity, environment] = world().fetchAt<CEnvironment>(0); envEntity)
    {
        if (auto [_, skyboxCache] = world().fetchFrom<CSkyboxCache>(envEntity); skyboxCache != nullptr && environment->skyboxMaterial != 0)
        {
            auto shaderRef = materialResource->getShaderRef(environment->skyboxMaterial);
            auto *uniforms = materialResource->getUniforms(environment->skyboxMaterial);

            shaderResource->bind(shaderRef, glm::mat4(glm::mat3(viewMatrix)), projMatrix, glm::mat4(1.0f));
            shaderResource->applyUniforms(shaderRef, uniforms, *textureResource);

            meshResource->draw(skyboxCache->meshRef);
        }
        else
        {
            services().get<Window>()->clearColor(environment->backgroundColor);
        }
    }

    /// ------- Render Meshes -------
    for (const auto &[entity, meshRenderer, transform] : world().fetch<CMeshRenderer, CTransformCache>())
    {
        auto meshRef = meshRenderer->meshRef;
        auto materialRef = meshRenderer->materialRef;

        auto *uniforms = materialResource->getUniforms(materialRef);
        auto shaderRef = materialResource->getShaderRef(materialRef);

        shaderResource->bind(shaderRef, viewMatrix, projMatrix,
                             transform->modelMatrix * meshResource->getLocalModel(meshRef));
        shaderResource->applyUniforms(shaderRef, uniforms, *textureResource);

        meshResource->draw(meshRef);
    }

    /// ------- Render Models -------
    for (const auto &[entity, modelRenderer, transform] : world().fetch<CModelRenderer, CTransformCache>())
    {
        auto &modelRef = modelRenderer->modelRef;

        modelResource->forEach(modelRef, [&](MeshRef meshRef, MaterialRef materialRef, size_t index)
                               {
            if (modelRenderer->materialOverrides.size() > index) {
                materialRef = modelRenderer->materialOverrides[index];
            }
            auto* uniforms  = materialResource->getUniforms(materialRef);
            auto  shaderRef = materialResource->getShaderRef(materialRef);

            shaderResource->bind(shaderRef, viewMatrix, projMatrix,
                                 transform->modelMatrix * meshResource->getLocalModel(meshRef));
            shaderResource->applyUniforms(shaderRef, uniforms, *textureResource);

            meshResource->draw(meshRef); });
    }
}
