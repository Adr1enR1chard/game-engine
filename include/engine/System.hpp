#pragma once

class EngineContext;
class Registry;
class Scene;

class System
{
  public:
    System()                                                            = default;
    virtual ~System()                                                   = default;
    virtual void update(EngineContext& engineContext, double deltaTime) = 0;
};