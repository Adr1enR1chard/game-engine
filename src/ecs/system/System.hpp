#pragma once

class EngineContext;

class System
{
public:
    virtual ~System() = default;
    virtual void update(EngineContext &engineContext) = 0;
};