#pragma once

#include "ecs/component/CComponent.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct CTransform : CComponent
{
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::quat rotation{0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};

    bool dirty = true;
};