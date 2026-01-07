#include <engine/system/TransformSystem.hpp>

#include <engine/SceneManager.hpp>
#include <engine/EngineContext.hpp>
#include <engine/component/CTransform.hpp>
#include <engine/component/CTransformCache.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "scene/Scene.hpp"

glm::mat4 getModelMatrix(CTransform &transform)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, transform.getPosition());
    model = glm::rotate(model, glm::radians(transform.getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(transform.getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, transform.getScale());
    return model;
}

glm::mat4 getViewMatrix(const glm::vec3 &position, const glm::vec3 &rotation)
{
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::rotate(view, glm::radians(-rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(-rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, glm::radians(-rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::translate(view, -position);
    return view;
}

void TransformSystem::update(EngineContext &engineContext, double deltaTime)
{
    Scene &scene = engineContext.getService<SceneManager>().currentScene();
    for (const Entity &entity : scene.registry().getEntitiesWithComponent<CTransform>())
    {
        auto &transform = scene.registry().getComponent<CTransform>(entity);
        if (transform.isDirty())
        {
            if (!scene.registry().hasComponent<CTransformCache>(entity))
            {
                scene.registry().createComponent<CTransformCache>(entity);
            }

            auto &transformCache = scene.registry().getComponent<CTransformCache>(entity);
            transformCache.modelMatrix = getModelMatrix(transform);
            transformCache.viewMatrix = getViewMatrix(transform.getPosition(), transform.getRotation());
            transform.setDirty(false);
        }
    }

    deltaTime; // Unused parameter
}