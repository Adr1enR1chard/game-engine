#include <engine/system/CameraSystem.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <engine/World.hpp>

#include <engine/Window.hpp>
#include <engine/component/CCamera.hpp>
#include <engine/component/CCameraCache.hpp>

glm::mat4 getProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void CameraSystem::update(World& world, double deltaTime)
{
    for (const Entity& entity : world.getEntitiesWithComponent<CCamera>()) {
        auto& cameraComponent = world.getComponent<CCamera>(entity);
        if (!cameraComponent.isActive)
            continue;

        if (!world.hasComponent<CCameraCache>(entity)) {
            world.createComponent<CCameraCache>(entity);
        }

        int width, height;
        world.Serv<Window>().getSize(width, height);

        auto& cameraCache = world.getComponent<CCameraCache>(entity);
        cameraCache.projectionMatrix =
            getProjectionMatrix(cameraComponent.fov, static_cast<float>(width) / static_cast<float>(height),
                                cameraComponent.nearPlane, cameraComponent.farPlane);

        cameraComponent.dirty = false;
        break; // Only one active camera is supported
    }

    deltaTime; // Unused parameter
}