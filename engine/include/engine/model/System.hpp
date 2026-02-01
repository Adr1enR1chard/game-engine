#pragma once

#include <variant>

#include <engine/registry/ServiceRegistry.hpp>
#include <engine/registry/World.hpp>

class System
{
public:
  System() {}
  virtual ~System() = default;
  virtual void init() {}
  virtual void start() {}
  virtual void preUpdate(float /*deltaTime*/) {}
  virtual void update(float /*deltaTime*/) {}
  virtual void preRender(float /*deltaTime*/) {}
  virtual void render(float /*deltaTime*/) {}

protected:
  World &world()
  {
    return *m_world;
  }
  ServiceRegistry &services()
  {
    return *m_services;
  }

private:
  friend class SystemRegistry;
  void setContext(World &world, ServiceRegistry &services)
  {
    this->m_world = &world;
    this->m_services = &services;
  }

private:
  World *m_world;
  ServiceRegistry *m_services;
};

template <typename T>
concept SystemType = std::derived_from<T, System>;