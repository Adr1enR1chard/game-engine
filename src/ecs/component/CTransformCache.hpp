#pragma once

#include <glm/glm.hpp>

#include "ecs/component/CComponent.hpp"

struct CTransformCache : CComponent
{
    glm::mat4 modelMatrix{1.0f};
    glm::mat4 viewMatrix{1.0f};
};