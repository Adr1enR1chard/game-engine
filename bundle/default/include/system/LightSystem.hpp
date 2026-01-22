#pragma once

#include <engine/model/System.hpp>

class LightSystem : public System
{
  public:
    void update(double deltaTime) override;
};
