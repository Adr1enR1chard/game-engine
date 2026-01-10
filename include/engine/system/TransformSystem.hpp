#pragma once

#include <engine/System.hpp>

class World;

class TransformSystem : public System
{
  public:
    void update(World& world, double deltaTime) override;
};