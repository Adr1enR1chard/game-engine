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

#include <engine/registry/World.hpp>

void RenderSystem::render(World& world, ServiceRegistry& /*services*/, double /*deltaTime*/)
{
    const auto& [cameraEntity, cameraCache, cameraTransform] = world.getAt<CCameraCache, CTransformCache>(0);

    if (!cameraEntity)
        return;

    /// ------- Render Environment -------
    if (const auto& [envEntity, environment] = world.getAt<CEnvironment>(0); envEntity) {
        if (auto [_, skyboxCache] = world.getFrom<CSkyboxCache>(envEntity); skyboxCache != nullptr) {
            glDepthMask(GL_FALSE);
            glDisable(GL_CULL_FACE);

            skyboxCache->skyboxMaterial.setup(glm::mat4(glm::mat3(cameraTransform->viewMatrix)),
                                              cameraCache->projectionMatrix);

            skyboxCache->skyboxMesh->Draw(skyboxCache->skyboxMaterial, glm::mat4(1.0f));

            glEnable(GL_CULL_FACE);
            glDepthMask(GL_TRUE);
        } else {
            // Clear to background color if no skybox is set
            glClearColor(environment->backgroundColor.r, environment->backgroundColor.g, environment->backgroundColor.b,
                         1.0f);
        }
    }

    /// ------- Render Meshes -------
    for (const auto& [entity, meshRenderer, transform] : world.get<CMeshRenderer, CTransformCache>()) {
        auto& mesh             = meshRenderer->mesh;
        auto& materialInstance = meshRenderer->material;

        materialInstance.setup(cameraTransform->viewMatrix, cameraCache->projectionMatrix);

        mesh->Draw(materialInstance, transform->modelMatrix);
    }

    /// ------- Render Models -------
    for (const auto& [entity, modelRenderer, transform] : world.get<CModelRenderer, CTransformCache>()) {
        auto& model = modelRenderer->model;

        model.draw(cameraTransform->viewMatrix, cameraCache->projectionMatrix, transform->modelMatrix);
    }
}
