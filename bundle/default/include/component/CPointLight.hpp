#pragma once

#include <glm/vec3.hpp>

namespace default_bundle
{

    struct CPointLight
    {
        glm::vec3 color = glm::vec3(1.0f);
        float intensity = 1.0f;
    };

} // namespace default_bundle