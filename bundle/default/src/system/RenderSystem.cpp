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

#include <model/Material.hpp>
#include <model/Mesh.hpp>

#include <service/resource/MaterialResource.hpp>
#include <service/resource/MeshResource.hpp>
#include <service/resource/ShaderResource.hpp>
#include <service/resource/TextureResource.hpp>

void RenderSystem::render(double /*deltaTime*/)
{
    const auto& [cameraEntity, cameraCache, cameraTransform] = world().getAt<CCameraCache, CTransformCache>(0);

    if (!cameraEntity)
        return;

    MeshResource*     meshResource     = services().get<MeshResource>();
    MaterialResource* materialResource = services().get<MaterialResource>();
    ShaderResource*   shaderResource   = services().get<ShaderResource>();

    glm::mat4 viewMatrix = cameraTransform->viewMatrix;
    glm::mat4 projMatrix = cameraCache->projectionMatrix;

    /// ------- Render Environment -------
    if (const auto& [envEntity, environment] = world().getAt<CEnvironment>(0); envEntity) {
        if (auto [_, skyboxCache] = world().getFrom<CSkyboxCache>(envEntity); skyboxCache != nullptr) {
            auto shaderRef = materialResource->getShaderRef(environment->skyboxMaterial);
            shaderResource->bind(shaderRef, materialResource->getUniforms(environment->skyboxMaterial),
                                 glm::mat4(glm::mat3(viewMatrix)), projMatrix, glm::mat4(1.0f));

            meshResource->draw(skyboxCache->meshRef);
        } else {
            // Clear to background color if no skybox is set
            glClearColor(environment->backgroundColor.r, environment->backgroundColor.g, environment->backgroundColor.b,
                         1.0f);
        }
    }

    /// ------- Render Meshes -------
    for (const auto& [entity, meshRenderer, transform] : world().get<CMeshRenderer, CTransformCache>()) {
        auto meshRef     = meshRenderer->meshRef;
        auto materialRef = meshRenderer->materialRef;

        auto* uniforms = materialResource->getUniforms(materialRef);

        auto shaderRef = materialResource->getShaderRef(materialRef);
        shaderResource->bind(shaderRef, uniforms, viewMatrix, projMatrix, transform->modelMatrix);

        meshResource->draw(meshRef);
    }

    /// ------- Render Models -------
    for (const auto& [entity, modelRenderer, transform] : world().get<CModelRenderer, CTransformCache>()) {
        auto& model = modelRenderer->model;

        model.draw(viewMatrix, projMatrix, transform->modelMatrix);
    }
}
