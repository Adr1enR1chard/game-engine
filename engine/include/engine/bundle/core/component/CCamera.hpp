#pragma once

#include <glm/glm.hpp>

namespace engine
{
    struct CCamera
    {
        float fov = 45.0f;
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;
        bool isActive = true;
    };

} // namespace engine