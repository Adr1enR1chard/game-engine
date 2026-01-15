#pragma once
#include <chrono>
#include <iostream>
#include <thread>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <engine/model/Bundle.hpp>
#include <engine/model/WindowService.hpp>
#include <engine/registry/ServiceRegistry.hpp>
#include <engine/registry/SystemRegistry.hpp>
#include <engine/registry/World.hpp>

#include <engine/utils/types.hpp>

class Engine
{

  public:
    Engine()  = default;
    ~Engine() = default;

    /**
     * Run the engine with the specified window service type.
     */
    template <WindowType T> void run(int width, int height, const char* title, bool fullscreen)
    {
        WindowService* window = tryWindow<T>();
        if (!window) {
            std::cerr
                << "No window service found in the engine. Please add the window service before running the engine."
                << std::endl;
            return;
        }

        window->init(width, height, title, fullscreen);

        using clock    = std::chrono::steady_clock;
        auto lastFrame = clock::now();

        m_systems.start(m_world, m_services);

        float deltaTime = 0.0f;

        while (!window->shouldClose()) {
            auto                          frameStart = clock::now();
            std::chrono::duration<double> delta      = frameStart - lastFrame;
            lastFrame                                = frameStart;

            deltaTime = static_cast<float>(delta.count());

            // --- Engine loop ---
            window->pollEvents();
            m_systems.input(m_world, m_services, deltaTime);
            m_systems.update(m_world, m_services, deltaTime);
            window->clear();
            m_systems.render(m_world, m_services, deltaTime);
            window->swapBuffers();
        }
    }

    /**
     * Use a bundle to add a set of systems to the engine.
     */
    template <BundleType B> Engine& addBundle()
    {
        std::unique_ptr<Bundle> bundle = std::make_unique<B>();
        bundle->apply(*this);

        m_bundles.push_back(std::move(bundle));

        return *this;
    }

    /**
     * Remove a bundle and its systems from the engine.
     */
    template <BundleType B> Engine& removeBundle()
    {
        auto it =
            std::find_if(m_bundles.begin(), m_bundles.end(), [](Bundle* b) { return dynamic_cast<B*>(b) != nullptr; });

        if (it != m_bundles.end()) {
            (*it)->remove(*this);
            m_bundles.erase(it);
        }

        return *this;
    }

    /**
     * Add systems to the engine.
     */
    template <SystemType... Ts> Engine& addSystems()
    {
        m_systems.add<Ts...>();

        return *this;
    }

    /**
     * Remove systems from the engine.
     */
    template <SystemType... Ts> Engine& removeSystems()
    {
        m_systems.remove<Ts...>();

        return *this;
    }

    /**
     * Add services to the engine.
     */
    template <ServiceType... Ts> Engine& addServices()
    {
        m_services.add<Ts...>();
        return *this;
    }

    /**
     * Remove services from the engine.
     */
    template <ServiceType... Ts> Engine& removeServices()
    {
        m_services.remove<Ts...>();
        return *this;
    }

  private:
    template <WindowType T> WindowService* tryWindow()
    {
        return m_services.get<T>();
    }

  private:
    World                                m_world;
    SystemRegistry                       m_systems;
    ServiceRegistry                      m_services;
    std::vector<std::unique_ptr<Bundle>> m_bundles;
};