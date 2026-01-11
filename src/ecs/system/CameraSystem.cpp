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
    for (const auto& [entity, cameraComponent] : world.get<CCamera>()) {
        if (!cameraComponent->isActive)
            continue;

        if (!world.has<CCameraCache>(entity)) {
            world.add(entity, CCameraCache{});
        }

        int width, height;
        world.Serv<Window>().getSize(width, height);

        const auto& [_, cameraCache] = world.getFrom<CCameraCache>(entity);
        cameraCache->projectionMatrix =
            getProjectionMatrix(cameraComponent->fov, static_cast<float>(width) / static_cast<float>(height),
                                cameraComponent->nearPlane, cameraComponent->farPlane);

        cameraComponent->dirty = false;
        break; // Only one active camera is supported
    }

    deltaTime; // Unused parameter
}