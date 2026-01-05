#include "TransformSystem.hpp"

#include "scene/Scene.hpp"

#include "ecs/component/CTransform.hpp"
#include "ecs/component/CTransformCache.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

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

void TransformSystem::update(Scene &scene, float deltaTime)
{
    for (const Entity &entity : scene.getRegistry().getEntitiesWithComponent<CTransform>())
    {
        auto &transform = scene.getRegistry().getComponent<CTransform>(entity);
        if (transform.dirty)
        {
            if (!scene.getRegistry().hasComponent<CTransformCache>(entity))
            {
                scene.getRegistry().createComponent<CTransformCache>(entity);
            }

            auto &transformCache = scene.getRegistry().getComponent<CTransformCache>(entity);
            transformCache.modelMatrix = getModelMatrix(transform);
            transform.dirty = false;
        }
    }

    deltaTime;
}