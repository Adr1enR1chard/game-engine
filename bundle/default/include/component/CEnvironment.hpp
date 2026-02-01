#pragma once

#include <glm/glm.hpp>
#include <engine/utils/RenderTypes.hpp>

namespace default_bundle
{

    struct CEnvironment
    {
        /// @brief The skybox material, overrides the background color when set
        engine::MaterialRef skyboxMaterial = 0;
        /// @brief  The background color used when no skybox is set
        glm::vec3 backgroundColor = glm::vec3(0.1f, 0.1f, 0.1f);
    };

} // namespace default_bundle