#pragma once

#include "System.hpp"

class EngineContext;

class CameraSystem : public System
{
public:
    void update(EngineContext &engineContext) override;
};