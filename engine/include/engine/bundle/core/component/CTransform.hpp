#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine
{

    /// @brief Component representing a dynamic entity's transform in 3D space.
    struct CTransform
    {
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};

        glm::vec3 forward() const
        {
            glm::vec3 dir = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
            return glm::normalize(dir);
        }

        glm::vec3 right() const
        {
            glm::vec3 dir = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
            return glm::normalize(dir);
        }

        glm::vec3 up() const
        {
            glm::vec3 dir = rotation * glm::vec3(0.0f, 1.0f, 0.0f);
            return glm::normalize(dir);
        }

        void rotate(float angle, const glm::vec3 &axis)
        {
            rotation = glm::rotate(rotation, angle, axis);
            rotation = glm::normalize(rotation);
        }

        void rotate(const glm::quat &quat)
        {
            rotation = glm::normalize(quat * rotation);
        }

        void setRotationFromEuler(const glm::vec3 &eulerAngles)
        {
            rotation = glm::quat(eulerAngles);
        }

        void setRotation(const glm::quat &quat)
        {
            rotation = glm::normalize(quat);
        }
    };

} // namespace engine