#pragma once

#include <glm/glm.hpp>

#include "ecs/component/CComponent.hpp"

class CCameraTransformCache : public CComponent
{
public:
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 projectionMatrix{1.0f};
};