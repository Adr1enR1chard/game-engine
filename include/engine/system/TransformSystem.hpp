#pragma once

#include <engine/System.hpp>

class TransformSystem : public System
{
public:
    void update(EngineContext &engineContext, double deltaTime) override;
};