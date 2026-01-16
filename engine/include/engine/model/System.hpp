#pragma once

class World;
class ServiceRegistry;

class System
{
  public:
    System()          = default;
    virtual ~System() = default;
    virtual void init(ServiceRegistry& /*services*/) {}
    virtual void start(World& /*world*/, ServiceRegistry& /*services*/) {}
    virtual void preUpdate(World& /*world*/, ServiceRegistry& /*services*/, double /*deltaTime*/) {}
    virtual void update(World& /*world*/, ServiceRegistry& /*services*/, double /*deltaTime*/) {}
    virtual void preRender(World& /*world*/, ServiceRegistry& /*services*/, double /*deltaTime*/) {}
    virtual void render(World& /*world*/, ServiceRegistry& /*services*/, double /*deltaTime*/) {}
    virtual void present(World& /*world*/, ServiceRegistry& /*services*/, double /*deltaTime*/) {}
};