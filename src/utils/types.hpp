#pragma once

#include <concepts>
#include <variant>

#include <glm/glm.hpp>

#include <engine/Bundle.hpp>
#include <engine/Service.hpp>
#include <engine/System.hpp>

template <typename T>
concept SystemType = std::derived_from<T, System>;

template <typename T>
concept ServiceType = std::derived_from<T, Service>;

struct UDirectionalLight {
    glm::vec3 direction;
    glm::vec3 color     = glm::vec3(1.0f);
    float     intensity = 1.0f;
};

struct UPointLight {
    glm::vec3 position;
    glm::vec3 color     = glm::vec3(1.0f);
    float     intensity = 1.0f;
};

using UniformValue =
    std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4, UDirectionalLight, UPointLight>;