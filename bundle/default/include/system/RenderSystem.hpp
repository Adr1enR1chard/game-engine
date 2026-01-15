#pragma once

#include <glm/glm.hpp>

#include <engine/model/System.hpp>

class World;
class ServiceRegistry;

class RenderSystem : public System
{
  public:
    void render(World& world, ServiceRegistry& services, double deltaTime) override;
};