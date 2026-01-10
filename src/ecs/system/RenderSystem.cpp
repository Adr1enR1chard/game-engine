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

void RenderSystem::update(World& world, double deltaTime)
{
    auto cameraEntities = world.getEntitiesWithComponents<CTransformCache, CCameraCache>();
    if (cameraEntities.empty())
        return;

    const auto& cameraCache     = world.getComponent<CCameraCache>(cameraEntities[0]);
    const auto& cameraTransform = world.getComponent<CTransformCache>(cameraEntities[0]);

    for (const Entity& entity : world.getEntitiesWithComponents<CMeshRenderer, CTransformCache>()) {
        auto& transform    = world.getComponent<CTransformCache>(entity);
        auto& meshRenderer = world.getComponent<CMeshRenderer>(entity);
        auto& model        = meshRenderer.model;
        auto& material     = meshRenderer.material;

        material->setUniform("view", cameraTransform.viewMatrix);
        material->setUniform("projection", cameraCache.projectionMatrix);
        material->setUniform("model", transform.modelMatrix);
        material->applyUniforms();

        material->bindTextures();

        // Debug vertices position
        // for (const auto &vertex : mesh.vertices)
        // {
        //     printf("Vertex position: %f, %f, %f\n", vertex.position.x, vertex.position.y, vertex.position.z);
        // }
        model.Draw(material->getShader(), transform.modelMatrix);
    }

    deltaTime; // Unused parameter
}
