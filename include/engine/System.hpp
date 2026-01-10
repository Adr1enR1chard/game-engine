#pragma once

class World;
class Registry;
class Scene;

class System
{
  public:
    System()          = default;
    virtual ~System() = default;
    virtual void start(World& /*world*/) {}
    virtual void update(World& /*world*/, double /*deltaTime*/) {}
};