#pragma once

#include <glm/glm.hpp>

#include <engine/bundle/core/service/factory/MaterialFactory.hpp>

namespace engine
{

    struct CEnvironment
    {
        /// @brief The skybox material, overrides the background color when set
        MaterialHandle skyboxMaterial;
        /// @brief  The background color used when no skybox is set
        glm::vec3 backgroundColor = glm::vec3(0.1f, 0.1f, 0.1f);
    };

} // namespace engine