#pragma once

#include <glm/glm.hpp>
#include <model/Skybox.hpp>

struct CEnvironment {
    /// @brief The skybox texture, overrides the background color when set
    Skybox skybox;
    /// @brief  The background color used when no skybox is set
    glm::vec3 backgroundColor = glm::vec3(0.1f, 0.1f, 0.1f);
};