#include <system/PlatformSystem.hpp>

#include <chrono>

#include <service/Input.hpp>
#include <service/Window.hpp>

void PlatformSystem::init()
{
    if (auto* window = services().get<Window>()) {
        window->create();
    }
}

void PlatformSystem::preUpdate(double deltaTime)
{
    if (auto* input = services().get<Input>()) {
        input->clear();
    }
    if (auto* window = services().get<Window>()) {
        window->pollEvents();
    }
}

void PlatformSystem::preRender(double deltaTime)
{
    if (auto* window = services().get<Window>()) {
        window->clear();
    }
}

void PlatformSystem::present(double deltaTime)
{
    if (auto* window = services().get<Window>()) {
        window->swapBuffers();
    }
}