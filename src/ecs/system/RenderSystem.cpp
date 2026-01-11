#include <engine/system/RenderSystem.hpp>

#include <glad/glad.h>
#include <stdio.h>
#include <string>

#include <engine/component/CCamera.hpp>
#include <engine/component/CCameraCache.hpp>
#include <engine/component/CMeshRenderer.hpp>
#include <engine/component/CTransformCache.hpp>

#include <engine/Material.hpp>
#include <engine/Mesh.hpp>

#include <engine/World.hpp>

void RenderSystem::update(World& world, double /*deltaTime*/)
{
    const auto& [cameraEntity, cameraCache, cameraTransform] = world.getAt<CCameraCache, CTransformCache>(0);

    if (!cameraEntity)
        return;

    for (const auto& [entity, meshRenderer, transform] : world.get<CMeshRenderer, CTransformCache>()) {
        auto& mesh             = meshRenderer->mesh;
        auto& materialInstance = meshRenderer->material;

        materialInstance.setup(cameraTransform->viewMatrix, cameraCache->projectionMatrix);

        mesh->Draw(materialInstance, transform->modelMatrix);
    }

    for (const auto& [entity, modelRenderer, transform] : world.get<CModelRenderer, CTransformCache>()) {
        auto& model            = modelRenderer->model;
        auto& materialInstance = modelRenderer->materialOverrides;

        if (!materialInstance.empty()) {
            materialInstance[0].setup(cameraTransform->viewMatrix, cameraCache->projectionMatrix);

            model.Draw(
                materialInstance[0],
                transform->modelMatrix); // TODO: single material for now, must be handled inside model class late
        }
    }
}
