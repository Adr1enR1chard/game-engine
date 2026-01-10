#pragma once

#include <engine/System.hpp>

class CameraSystem : public System
{
  public:
    void update(World& engineContext, double deltaTime) override;
};