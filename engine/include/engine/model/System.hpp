#pragma once

#include <variant>
#include <concepts>

#include <engine/registry/ServiceRegistry.hpp>
#include <engine/registry/World.hpp>

namespace engine
{

  class World;
  class ServiceRegistry;

  class System
  {
  public:
    System() = default;
    virtual ~System() = default;
    virtual void start() {}
    virtual void update(float /*deltaTime*/) {}

    void setContext(World &world, ServiceRegistry &services)
    {
      m_world = &world;
      m_services = &services;
    }

  protected:
    World &world();
    ServiceRegistry &services();

  private:
    World *m_world;
    ServiceRegistry *m_services;
  };

  template <typename T>
  concept SystemType = std::derived_from<T, System>;

} // namespace engine