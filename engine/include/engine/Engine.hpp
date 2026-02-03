#pragma once
#include <chrono>
#include <iostream>
#include <source_location>
#include <thread>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <engine/model/Bundle.hpp>
#include <engine/registry/ServiceRegistry.hpp>
#include <engine/registry/SystemRegistry.hpp>
#include <engine/registry/World.hpp>

#include <engine/utils/Log.hpp>
#include <engine/utils/types.hpp>
#include "threading/ThreadQueue.hpp"

// Platform Services
#include <engine/service/platform/Window.hpp>
#include <engine/service/platform/Input.hpp>
#include <engine/service/platform/Renderer.hpp>

namespace engine
{

    class Engine
    {

    public:
        static Engine &Init()
        {
            if (!m_instance)
            {
                m_instance.reset(new Engine());
            }
            return *m_instance;
        }

        static void Shutdown()
        {
            if (!m_instance)
                return;

            Log::Print("Engine Shutdown", LogLevel::Info);
            m_instance->m_running = false;
        }

        static void ExecuteOnMainThread(const std::function<void()> &task)
        {
            if (m_instance)
            {
                m_instance->m_mainThreadQueue.enqueue(task);
            }
        }

    private:
        Engine();

        static std::unique_ptr<Engine> m_instance;

    public:
        Engine(const Engine &) = delete;
        Engine &operator=(const Engine &) = delete;

        void run(unsigned int width, unsigned int height, const char *title, bool fullscreen = false)
        {
            m_window->setResolution(width, height);
            m_window->setTitle(title);
            m_window->setFullscreen(fullscreen);

            run();
        }

        void run()
        {
            m_window->create();

            m_systems.setContext(m_world, m_services);
            m_systems.init();

            m_running = true;

            using clock = std::chrono::steady_clock;
            auto lastFrame = clock::now();

            m_systems.start();

            float deltaTime = 0.0f;

            while (m_running)
            {
                auto frameStart = clock::now();
                std::chrono::duration<double> delta = frameStart - lastFrame;
                lastFrame = frameStart;

                deltaTime = static_cast<float>(delta.count());

                // --- Engine loop ---
                m_input->clear();
                m_window->pollEvents();
                m_systems.preUpdate(deltaTime);
                m_systems.update(deltaTime);
                m_window->clear();
                m_systems.preRender(deltaTime);
                m_systems.render(deltaTime);
                m_window->swapBuffers();

                m_mainThreadQueue.executeAll();
            }
        }

        /**
         * Use a bundle to add a set of systems to the engine.
         */
        template <BundleType B>
        Engine &addBundle()
        {
            std::unique_ptr<Bundle> bundle = std::make_unique<B>();
            bundle->install(m_systems, m_services);

            m_bundles.push_back(std::move(bundle));

            return *this;
        }

        /**
         * Remove a bundle and its systems from the engine.
         */
        template <BundleType B>
        Engine &removeBundle()
        {
            auto it =
                std::find_if(m_bundles.begin(), m_bundles.end(), [](Bundle *b)
                             { return dynamic_cast<B *>(b) != nullptr; });

            if (it != m_bundles.end())
            {
                (*it)->uninstall(m_systems, m_services);
                m_bundles.erase(it);
            }

            return *this;
        }

        /**
         * Add systems to the engine.
         */
        template <SystemType... Ts>
        Engine &addSystems()
        {
            m_systems.add<Ts...>();

            return *this;
        }

        /**
         * Remove systems from the engine.
         */
        template <SystemType... Ts>
        Engine &removeSystems()
        {
            m_systems.remove<Ts...>();

            return *this;
        }

        /**
         * Add services to the engine.
         */
        template <ServiceType... Ts>
        Engine &addServices()
        {
            m_services.add<Ts...>();
            return *this;
        }

        /**
         * Remove services from the engine.
         */
        template <ServiceType... Ts>
        Engine &removeServices()
        {
            m_services.remove<Ts...>();
            return *this;
        }

    private:
        bool m_running = false;
        World m_world;
        SystemRegistry m_systems;
        ServiceRegistry m_services;
        std::vector<std::unique_ptr<Bundle>> m_bundles;
        ThreadQueue m_mainThreadQueue;
        Window *m_window = nullptr;
        Input *m_input = nullptr;
    };

} // namespace engine