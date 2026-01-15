#include <system/CameraSystem.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <engine/registry/ServiceRegistry.hpp>
#include <engine/registry/World.hpp>

#include <component/CCamera.hpp>
#include <component/CCameraCache.hpp>
#include <service/Window.hpp>

glm::mat4 getProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void CameraSystem::update(World& world, ServiceRegistry& services, double deltaTime)
{
    for (const auto& [entity, cameraComponent] : world.get<CCamera>()) {
        if (!cameraComponent->isActive)
            continue;

        if (!world.has<CCameraCache>(entity)) {
            world.add(entity, CCameraCache{});
        }

        int width, height;
        services.get<Window>()->getSize(width, height);

        const auto& [_, cameraCache] = world.getFrom<CCameraCache>(entity);
        cameraCache->projectionMatrix =
            getProjectionMatrix(cameraComponent->fov, static_cast<float>(width) / static_cast<float>(height),
                                cameraComponent->nearPlane, cameraComponent->farPlane);

        cameraComponent->dirty = false;
        break; // Only one active camera is supported
    }

    deltaTime; // Unused parameter
}