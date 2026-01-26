#pragma once

#include <glm/glm.hpp>

struct CCamera
{
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    bool isActive = true;
};