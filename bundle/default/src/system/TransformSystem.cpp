#include <system/TransformSystem.hpp>

#include <component/CTransform.hpp>
#include <component/cache/CTransformCache.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace default_bundle
{

    glm::mat4 getModelMatrix(CTransform &transform)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, transform.position);
        model *= glm::mat4_cast(transform.rotation);
        model = glm::scale(model, transform.scale);
        return model;
    }

    glm::mat4 getViewMatrix(const glm::vec3 &position, const glm::quat &rotation)
    {
        glm::mat4 view = glm::mat4(1.0f);
        view *= glm::mat4_cast(glm::conjugate(rotation));
        view = glm::translate(view, -position);
        return view;
    }

    void TransformSystem::update(float /*deltaTime*/)
    {
        for (const auto &[entity, transform] : world().fetch<CTransform>())
        {
            if (!world().hasComponents<CTransformCache>(entity))
            {
                world().addComponents(entity, CTransformCache{});
            }

            const auto &[_, transformCache] = world().fetchFrom<CTransformCache>(entity);
            transformCache->modelMatrix = getModelMatrix(*transform);
            transformCache->viewMatrix = getViewMatrix(transform->position, transform->rotation);
        }
    }

} // namespace default_bundle