#pragma once

#include <glm/glm.hpp>

namespace engine
{

    struct CTransformCache
    {
        glm::mat4 modelMatrix{1.0f};
        glm::mat4 viewMatrix{1.0f};
    };

} // namespace engine