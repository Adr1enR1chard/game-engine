#pragma once

#include <engine/model/System.hpp>

class CameraSystem : public System
{
  public:
    void update(double deltaTime) override;
};