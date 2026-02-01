#pragma once

#include <glm/glm.hpp>

namespace default_bundle
{

    struct CDirectionalLight
    {
        glm::vec3 direction;
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        float ambient = 0.1f;
        float intensity = 0.8f;
    };

} // namespace default_bundle