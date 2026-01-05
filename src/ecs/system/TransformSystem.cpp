#include "TransformSystem.hpp"

#include "scene/Scene.hpp"

#include "ecs/component/CTransform.hpp"
#include "ecs/component/CTransformCache.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <engine/SceneManager.hpp>

#include <engine/EngineContext.hpp>

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

void TransformSystem::update(EngineContext &engineContext)
{
    Scene &scene = engineContext.getService<SceneManager>().currentScene();
    for (const Entity &entity : scene.registry().getEntitiesWithComponent<CTransform>())
    {
        auto &transform = scene.registry().getComponent<CTransform>(entity);
        if (transform.dirty)
        {
            if (!scene.registry().hasComponent<CTransformCache>(entity))
            {
                scene.registry().createComponent<CTransformCache>(entity);
            }

            auto &transformCache = scene.registry().getComponent<CTransformCache>(entity);
            transformCache.modelMatrix = getModelMatrix(transform);
            transform.dirty = false;
        }
    }
}