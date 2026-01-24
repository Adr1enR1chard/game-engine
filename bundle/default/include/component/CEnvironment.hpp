#pragma once

#include <glm/glm.hpp>
#include <utils/RenderTypes.hpp>

struct CEnvironment {
    /// @brief The skybox material, overrides the background color when set
    MaterialRef skyboxMaterial = 0;
    /// @brief  The background color used when no skybox is set
    glm::vec3 backgroundColor = glm::vec3(0.1f, 0.1f, 0.1f);
};