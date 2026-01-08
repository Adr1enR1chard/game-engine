#pragma once

#include <engine/System.hpp>

class LightSystem : public System
{
public:
    void update(EngineContext &engineContext, double deltaTime) override;
};
