#include "RenderableSystem.hpp"
#include "scene/Scene.hpp"

glm::mat4 getModelMatrix(const CTransform &transform)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, transform.position);
    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, transform.scale);
    return model;
}

void RenderableSystem::update(Scene &scene, float deltaTime)
{
    Registry &registry = scene.getRegistry();

    for (const Entity &entity : registry.getEntitiesWithComponents<CMeshRenderer, CTransform>())
    {
        auto &transform = registry.getComponent<CTransform>(entity);
        auto &meshRenderer = registry.getComponent<CMeshRenderer>(entity);

        if (!transform.isValid() || !meshRenderer.isValid())
            continue;

        meshRenderer.shader->use();
        meshRenderer.shader->setMat4("model", getModelMatrix(transform));

        if (meshRenderer.mesh)
        {
            glBindVertexArray(meshRenderer.mesh->VAO);
            glDrawArrays(GL_TRIANGLES, 0, meshRenderer.mesh->getVertexCount());
        }
    }

    deltaTime;
}
