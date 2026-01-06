#pragma once

#include <glm/glm.hpp>

#include "ecs/component/CComponent.hpp"

struct CCamera : CComponent
{
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    bool isActive = true;

private:
    bool dirty = true;

    friend class CameraSystem;
};