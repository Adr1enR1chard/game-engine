#include <system/CameraSystem.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <component/CCamera.hpp>
#include <component/cache/CCameraCache.hpp>
#include <engine/bundle/core/service/Renderer.hpp>

namespace default_bundle
{
    using namespace engine;

    glm::mat4 getProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
    {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    void CameraSystem::update(float /*deltaTime*/)
    {
        if (auto [entity, cameraComponent] = world().fetchAt<CCamera>(0); entity)
        {
            if (!world().hasComponents<CCameraCache>(entity))
            {
                world().addComponents(entity, CCameraCache{});
            }

            int width, height;
            services().get<Renderer>()->getViewportSize(width, height);
            if (height == 0 || width == 0)
                return;

            const auto &[_, cameraCache] = world().fetchFrom<CCameraCache>(entity);
            cameraCache->projectionMatrix =
                getProjectionMatrix(cameraComponent->fov, static_cast<float>(width) / static_cast<float>(height),
                                    cameraComponent->nearPlane, cameraComponent->farPlane);
        }
    }

} // namespace default_bundle