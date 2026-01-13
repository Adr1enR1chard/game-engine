#pragma once
#include <chrono>
#include <thread>

#include <engine/Bundle.hpp>
#include <engine/World.hpp>

#include "ecs/system/SystemScheduler.hpp"
#include "utils/types.hpp"

class Engine
{
  public:
    Engine(int width, int height, const char* title, bool fullscreen = false);
    ~Engine();

    void run();

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