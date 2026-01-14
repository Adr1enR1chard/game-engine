#pragma once

#include <glm/glm.hpp>

#include <engine/System.hpp>

class World;
class ServiceRegistry;

class RenderSystem : public System
{
  public:
    void render(World& world, ServiceRegistry& services, double deltaTime) override;
};