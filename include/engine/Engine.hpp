#pragma once
#include <chrono>
#include <thread>

#include <engine/Window.hpp>
#include <engine/World.hpp>

#include "ecs/system/SystemScheduler.hpp"

class Engine
{
  public:
    Engine(int width, int height, const char* title)
    {
        m_world.Serv<Window>().init(width, height, title);
    }
    ~Engine() = default;

    void run()
    {
        Window& window = m_world.Serv<Window>();

        using clock  = std::chrono::steady_clock;
        using frames = std::chrono::duration<int, std::ratio<1, 144>>;

        auto lastFrame = clock::now();

        while (!window.shouldClose()) {
            auto                          frameStart = clock::now();
            std::chrono::duration<double> delta      = frameStart - lastFrame;
            lastFrame                                = frameStart;

            // --- Engine loop ---
            window.clear();

            m_systemScheduler.updateSystems(m_world, delta.count());

            window.swapBuffers();
            window.pollEvents();
        }
    }

    World& world()
    {
        return m_world;
    }

    SystemScheduler& systems()
    {
        return m_systemScheduler;
    }

  private:
    World           m_world;
    SystemScheduler m_systemScheduler;
};