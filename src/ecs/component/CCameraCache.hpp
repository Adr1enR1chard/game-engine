#pragma once

#include <glm/glm.hpp>

#include "ecs/component/CComponent.hpp"

struct CCameraCache : public CComponent
{
    glm::mat4 projectionMatrix{1.0f};
};