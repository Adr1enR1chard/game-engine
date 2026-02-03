#pragma once

#include <glm/glm.hpp>

#include <service/factory/MaterialFactory.hpp>

namespace default_bundle
{

    struct CEnvironment
    {
        /// @brief The skybox material, overrides the background color when set
        Material skyboxMaterial;
        /// @brief  The background color used when no skybox is set
        glm::vec3 backgroundColor = glm::vec3(0.1f, 0.1f, 0.1f);
    };

} // namespace default_bundle