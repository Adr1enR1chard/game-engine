#pragma once

#include <engine/model/System.hpp>

class PlatformSystem : public System
{
  public:
    PlatformSystem()           = default;
    ~PlatformSystem() override = default;
    void init() override;
    void preUpdate(double deltaTime) override;
    void preRender(double deltaTime) override;
    void present(double deltaTime) override;
};