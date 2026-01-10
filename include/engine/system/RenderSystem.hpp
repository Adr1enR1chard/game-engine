#pragma once

#include <glm/glm.hpp>

#include <engine/System.hpp>

class RenderSystem : public System
{
  public:
    void update(World& engineContext, double deltaTime) override;
};