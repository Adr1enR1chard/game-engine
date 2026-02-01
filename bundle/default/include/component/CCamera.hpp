#pragma once

#include <glm/glm.hpp>

namespace default_bundle
{

    struct CCamera
    {
        float fov = 45.0f;
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;
        bool isActive = true;
    };

} // namespace default_bundle