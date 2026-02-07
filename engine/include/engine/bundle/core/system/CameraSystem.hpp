#pragma once

#include <engine/model/System.hpp>

namespace engine
{

  class CameraSystem : public engine::System
  {
  public:
    void update(float deltaTime) override;
  };

} // namespace engine