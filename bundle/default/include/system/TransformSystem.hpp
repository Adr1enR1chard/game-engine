#pragma once

#include <engine/model/System.hpp>

class TransformSystem : public System
{
public:
  void update(float deltaTime) override;
};