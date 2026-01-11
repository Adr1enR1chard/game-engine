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
        auto& model    = meshRenderer->model;
        auto& material = meshRenderer->material;

        material->setUniform("view", cameraTransform->viewMatrix);
        material->setUniform("projection", cameraCache->projectionMatrix);
        material->setUniform("model", transform->modelMatrix);
        material->applyUniforms();

        material->bindTextures();

        model.Draw(material->getShader(), transform->modelMatrix);
    }
}
