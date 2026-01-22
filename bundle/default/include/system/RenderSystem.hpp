#pragma once

#include <glm/glm.hpp>

#include <engine/model/System.hpp>

class RenderSystem : public System
{
  public:
    void render(double deltaTime) override;
};