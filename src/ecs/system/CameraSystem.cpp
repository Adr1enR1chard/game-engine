#include "CameraSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <engine/EngineContext.hpp>
#include <engine/SceneManager.hpp>
#include "scene/Scene.hpp"
#include "ecs/registry/Registry.hpp"

#include "ecs/component/CCamera.hpp"
#include "ecs/component/CCameraTransformCache.hpp"
#include "ecs/component/CCameraTransform.hpp"
#include "engine/Window.hpp"

glm::mat4 getViewMatrix(const glm::vec3 &position, const glm::vec3 &rotation)
{
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::rotate(view, glm::radians(-rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(-rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, glm::radians(-rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::translate(view, -position);
    return view;
}

glm::mat4 getProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void CameraSystem::update(EngineContext &engineContext)
{
    Registry &registry = engineContext.registry();

    for (const Entity &entity : registry.getEntitiesWithComponents<CCamera, CCameraTransform>())
    {
        auto &cameraComponent = registry.getComponent<CCamera>(entity);
        if (!cameraComponent.isActive)
            continue;

        auto &cameraTransform = registry.getComponent<CCameraTransform>(entity);
        if (!cameraTransform.isDirty)
            break;

        if (!registry.hasComponent<CCameraTransformCache>(entity))
        {
            registry.createComponent<CCameraTransformCache>(entity);
        }

        int width, height;
        engineContext.getService<Window>().getFramebufferSize(width, height);

        auto &cameraTransformCache = registry.getComponent<CCameraTransformCache>(entity);
        cameraTransformCache.viewMatrix = getViewMatrix(cameraTransform.position, cameraTransform.rotation);
        cameraTransformCache.projectionMatrix = getProjectionMatrix(
            cameraComponent.fov,
            static_cast<float>(width) / static_cast<float>(height),
            cameraComponent.nearPlane,
            cameraComponent.farPlane);
        cameraTransform.isDirty = false;
    }
}