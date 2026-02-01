#pragma once

#include <glm/glm.hpp>

namespace default_bundle
{

    struct CTransformCache
    {
        glm::mat4 modelMatrix{1.0f};
        glm::mat4 viewMatrix{1.0f};
    };

} // namespace default_bundle