#include "RenderSystem.hpp"

#include <stdio.h>
#include <glad/glad.h>

#include "scene/Scene.hpp"

#include "ecs/component/CMeshRenderer.hpp"
#include "ecs/component/CTransformCache.hpp"

void RenderSystem::update(Scene &scene, float deltaTime)
{
    Registry &registry = scene.getRegistry();

    for (const Entity &entity : registry.getEntitiesWithComponents<CMeshRenderer, CTransformCache>())
    {
        auto &transform = registry.getComponent<CTransformCache>(entity);
        auto &meshRenderer = registry.getComponent<CMeshRenderer>(entity);

        if (!meshRenderer.shader || !meshRenderer.mesh)
        {
            continue;
        }

        meshRenderer.shader->use();
        meshRenderer.shader->setMat4("model", transform.modelMatrix);

        if (meshRenderer.mesh)
        {
            glBindVertexArray(meshRenderer.mesh->VAO);
            glDrawArrays(GL_TRIANGLES, 0, meshRenderer.mesh->getVertexCount());
        }
    }

    deltaTime;
}
