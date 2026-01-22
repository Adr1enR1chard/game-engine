#include <system/TransformSystem.hpp>

#include <component/CTransform.hpp>
#include <component/cache/CTransformCache.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

glm::mat4 getModelMatrix(CTransform& transform)
{
    glm::mat4 model = glm::mat4(1.0f);
    model           = glm::translate(model, transform.position);
    model           = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model           = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model           = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model           = glm::scale(model, transform.scale);
    return model;
}

glm::mat4 getViewMatrix(const glm::vec3& position, const glm::vec3& rotation)
{
    glm::mat4 view = glm::mat4(1.0f);
    view           = glm::rotate(view, glm::radians(-rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    view           = glm::rotate(view, glm::radians(-rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    view           = glm::rotate(view, glm::radians(-rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    view           = glm::translate(view, -position);
    return view;
}

void TransformSystem::update(double /*deltaTime*/)
{
    for (const auto& [entity, transform] : world().get<CTransform>()) {
        if (!world().has<CTransformCache>(entity)) {
            world().add(entity, CTransformCache{});
        }

        const auto& [_, transformCache] = world().getFrom<CTransformCache>(entity);
        transformCache->modelMatrix     = getModelMatrix(*transform);
        transformCache->viewMatrix      = getViewMatrix(transform->position, transform->rotation);
    }
}