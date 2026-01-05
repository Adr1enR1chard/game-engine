#pragma once

#include "ecs/system/System.hpp"

class TransformSystem : public System
{
public:
    void update(Scene &scene, float deltaTime) override;
};