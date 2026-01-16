#pragma once

#include <engine/model/System.hpp>

class PlatformSystem : public System
{
  public:
    PlatformSystem()           = default;
    ~PlatformSystem() override = default;
    void init(ServiceRegistry& services) override;
    void preUpdate(World& world, ServiceRegistry& services, double deltaTime) override;
    void preRender(World& world, ServiceRegistry& services, double deltaTime) override;
    void present(World& world, ServiceRegistry& services, double deltaTime) override;
};