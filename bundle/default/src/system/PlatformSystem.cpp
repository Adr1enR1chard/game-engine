#include <system/PlatformSystem.hpp>

#include <chrono>

#include <engine/registry/ServiceRegistry.hpp>
#include <engine/registry/World.hpp>

#include <service/Input.hpp>
#include <service/Window.hpp>

void PlatformSystem::init(ServiceRegistry& services)
{
    if (auto* window = services.get<Window>()) {
        window->create();
    }
}

void PlatformSystem::preUpdate(World& world, ServiceRegistry& services, double deltaTime)
{
    if (auto* input = services.get<Input>()) {
        input->clear();
    }
    if (auto* window = services.get<Window>()) {
        window->pollEvents();
    }
}

void PlatformSystem::preRender(World& world, ServiceRegistry& services, double deltaTime)
{
    if (auto* window = services.get<Window>()) {
        window->clear();
    }
}

void PlatformSystem::present(World& world, ServiceRegistry& services, double deltaTime)
{
    if (auto* window = services.get<Window>()) {
        window->swapBuffers();
    }
}