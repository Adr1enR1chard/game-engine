#include <engine/system/CameraSystem.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <engine/EngineContext.hpp>
#include <engine/SceneManager.hpp>
#include "scene/Scene.hpp"
#include "ecs/Registry.hpp"

#include <engine/component/CCamera.hpp>
#include <engine/component/CCameraCache.hpp>
#include <engine/Window.hpp>

glm::mat4 getProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void CameraSystem::update(EngineContext &engineContext, double deltaTime)
{
    Registry &registry = engineContext.registry();

    for (const Entity &entity : registry.getEntitiesWithComponent<CCamera>())
    {
        auto &cameraComponent = registry.getComponent<CCamera>(entity);
        if (!cameraComponent.isActive)
            continue;

        if (!registry.hasComponent<CCameraCache>(entity))
        {
            registry.createComponent<CCameraCache>(entity);
        }

        int width, height;
        engineContext.getService<Window>().getSize(width, height);

        auto &cameraCache = registry.getComponent<CCameraCache>(entity);
        cameraCache.projectionMatrix = getProjectionMatrix(
            cameraComponent.fov,
            static_cast<float>(width) / static_cast<float>(height),
            cameraComponent.nearPlane,
            cameraComponent.farPlane);

        cameraComponent.dirty = false;
        break; // Only one active camera is supported
    }

    deltaTime; // Unused parameter
}