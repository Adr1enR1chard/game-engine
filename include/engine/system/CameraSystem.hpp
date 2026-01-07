#pragma once

#include <engine/System.hpp>

class CameraSystem : public System
{
public:
    void update(EngineContext &engineContext, double deltaTime) override;
};