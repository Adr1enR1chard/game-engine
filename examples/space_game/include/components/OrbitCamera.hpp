#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct COrbitCamera
{
    glm::quat pitchRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::quat yawRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
};