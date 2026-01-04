#pragma once

#include "ecs/component/CComponent.hpp"

#include <glm/glm.hpp>

struct CTransform : CComponent
{
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec3 rotation{0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};

    inline const bool isValid() const override
    {
        return true;
    }
};