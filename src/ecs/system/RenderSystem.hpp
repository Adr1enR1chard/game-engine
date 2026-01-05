#pragma once

#include <glm/glm.hpp>

#include "ecs/system/System.hpp"

class RenderSystem : public System
{
public:
    void update(EngineContext &engineContext) override;
};