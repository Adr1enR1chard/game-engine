#pragma once

#include <engine/System.hpp>

class LightSystem : public System
{
  public:
    void update(World& engineContext, double deltaTime) override;
};
