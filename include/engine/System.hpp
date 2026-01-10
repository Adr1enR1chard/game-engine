#pragma once

class World;
class Registry;
class Scene;

class System
{
  public:
    System()                                            = default;
    virtual ~System()                                   = default;
    virtual void update(World& world, double deltaTime) = 0;
};