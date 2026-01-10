#include <engine/system/RenderSystem.hpp>

#include <stdio.h>
#include <string>
#include <glad/glad.h>

#include <engine/EngineContext.hpp>
#include <engine/SceneManager.hpp>

#include <engine/component/CMeshRenderer.hpp>
#include <engine/component/CTransformCache.hpp>
#include <engine/component/CCamera.hpp>
#include <engine/component/CCameraCache.hpp>

#include <engine/Mesh.hpp>
#include <engine/Material.hpp>

#include <scene/Scene.hpp>

void RenderSystem::update(EngineContext &engineContext, double deltaTime)
{
    Registry &registry = engineContext.registry();
    auto cameraEntities = registry.getEntitiesWithComponents<CTransformCache, CCameraCache>();
    if (cameraEntities.empty())
        return;

    const auto &cameraCache = registry.getComponent<CCameraCache>(cameraEntities[0]);
    const auto &cameraTransform = registry.getComponent<CTransformCache>(cameraEntities[0]);

    for (const Entity &entity : registry.getEntitiesWithComponents<CMeshRenderer, CTransformCache>())
    {
        auto &transform = registry.getComponent<CTransformCache>(entity);
        auto &meshRenderer = registry.getComponent<CMeshRenderer>(entity);

        auto &model = meshRenderer.getModel();
        auto &material = meshRenderer.getMaterial();

        material.setUniform("view", cameraTransform.viewMatrix);
        material.setUniform("projection", cameraCache.projectionMatrix);
        material.setUniform("model", transform.modelMatrix);
        material.applyUniforms();

        material.bindTextures();

        // Debug vertices position
        // for (const auto &vertex : mesh.vertices)
        // {
        //     printf("Vertex position: %f, %f, %f\n", vertex.position.x, vertex.position.y, vertex.position.z);
        // }
        model.Draw(material.getShader(), transform.modelMatrix);
    }

    deltaTime; // Unused parameter
}
