#pragma once

#include <engine/model/System.hpp>

namespace default_bundle
{

  class CameraSystem : public engine::System
  {
  public:
    void update(float deltaTime) override;
  };

} // namespace default_bundle