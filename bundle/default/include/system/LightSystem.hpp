#pragma once

#include <engine/model/System.hpp>

class World;
class ServiceRegistry;

class LightSystem : public System
{
  public:
    void update(World& world, ServiceRegistry& services, double deltaTime) override;
};
