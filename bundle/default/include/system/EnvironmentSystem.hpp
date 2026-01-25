#pragma once

#include <engine/model/System.hpp>

class EnvironmentSystem : public System
{
  public:
    EnvironmentSystem()           = default;
    ~EnvironmentSystem() override = default;

    void start() override;
    void render(double deltaTime) override;

  private:
    bool m_clearColorUpdated = false;
    bool m_skyboxUpdated     = false;
};