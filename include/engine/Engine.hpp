#pragma once
#include <chrono>
#include <thread>

#include <engine/Bundle.hpp>
#include <engine/Window.hpp>
#include <engine/World.hpp>

#include "ecs/system/SystemScheduler.hpp"
#include "utils/types.hpp"

class Engine
{
  public:
    Engine(int width, int height, const char* title, bool fullscreen = false)
    {
        m_world.Serv<Window>().init(width, height, title, fullscreen);
    }
    ~Engine()
    {
        for (auto& bundle : m_bundles) {
            delete bundle;
        }
    };

    void run()
    {
        Window& window = m_world.Serv<Window>();

        using clock  = std::chrono::steady_clock;
        using frames = std::chrono::duration<int, std::ratio<1, 144>>;

        auto lastFrame = clock::now();

        m_systemScheduler.startSystems(m_world);

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

    template <BundleType B> Engine& use()
    {
        B* bundle = new B();
        bundle->apply(m_systemScheduler);

        m_bundles.push_back(std::move(bundle));

        return *this;
    }

    template <SystemType... T> Engine& add()
    {
        (m_systemScheduler.registerSystem<T>(), ...);
        return *this;
    }

    template <SystemType... T> Engine& remove()
    {
        (m_systemScheduler.unregisterSystem<T>(), ...);
        return *this;
    }

  private:
    World                m_world;
    SystemScheduler      m_systemScheduler;
    std::vector<Bundle*> m_bundles;
};