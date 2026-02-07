#pragma once

#include <engine/model/System.hpp>

namespace engine
{

  class TransformSystem : public engine::System
  {
  public:
    void update(float deltaTime) override;
  };

} // namespace engine