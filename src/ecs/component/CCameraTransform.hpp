#pragma once

#include <glm/glm.hpp>

#include "ecs/component/CComponent.hpp"

class CCameraTransform : public CComponent
{
public:
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec3 rotation{0.0f, 0.0f, 0.0f};

    bool isDirty = true;
};