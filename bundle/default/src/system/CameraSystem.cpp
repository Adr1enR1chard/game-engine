#include <system/CameraSystem.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <component/CCamera.hpp>
#include <component/cache/CCameraCache.hpp>
#include <engine/service/platform/Window.hpp>

glm::mat4 getProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void CameraSystem::update(double /*deltaTime*/)
{
    if (auto [entity, cameraComponent] = world().fetchAt<CCamera>(0); entity)
    {
        if (!world().hasComponents<CCameraCache>(entity))
        {
            world().addComponents(entity, CCameraCache{});
        }

        // TODO: Use events to only update when necessary
        int width, height;
        services().get<Window>()->getSize(width, height);

        const auto &[_, cameraCache] = world().fetchFrom<CCameraCache>(entity);
        cameraCache->projectionMatrix =
            getProjectionMatrix(cameraComponent->fov, static_cast<float>(width) / static_cast<float>(height),
                                cameraComponent->nearPlane, cameraComponent->farPlane);
    }
}