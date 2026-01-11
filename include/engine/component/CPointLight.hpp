#pragma once

#include <glm/vec3.hpp>

struct CPointLight {
    glm::vec3 color     = glm::vec3(1.0f);
    float     intensity = 1.0f;
};