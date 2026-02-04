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

#include <engine/bundle/standalone/StandaloneBundle.hpp>

namespace engine
{

    class Engine
    {

    public:
        static Engine &InitiliazeStandalone()
        {
            if (!m_instance)
            {
                m_instance.reset(new Engine());
            }

            m_instance->addBundle<StandaloneBundle>();

            return *m_instance;
        }

        static Engine &InitializeEmbedded(World *&outWorld, SystemRegistry *&outSystems, ServiceRegistry *&outServices)
        {
            if (!m_instance)
            {
                m_instance.reset(new Engine());
            }

            outWorld = &m_instance->m_world;
            outSystems = &m_instance->m_systems;
            outServices = &m_instance->m_services;

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
        Engine() : m_running(false) {}

        static std::unique_ptr<Engine> m_instance;

    public:
        Engine(const Engine &) = delete;
        Engine &operator=(const Engine &) = delete;

        void run(unsigned int width, unsigned int height, const char *title, bool fullscreen = false)
        {
            auto *window = m_services.get<Window>();
            auto *input = m_services.get<Input>();

            if (!window || !input)
            {
                Log::Print("Engine must be initialized with StandaloneBundle before running. Use Engine::InitiliazeStandalone().", LogLevel::Critical);
                throw std::runtime_error("Engine must be initialized with StandaloneBundle before running.");
                return;
            }

            window->setResolution(width, height);
            window->setTitle(title);
            window->setFullscreen(fullscreen);
            window->create();

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
                input->clear();
                window->pollEvents();
                m_systems.preUpdate(deltaTime);
                m_systems.update(deltaTime);
                auto *renderer = m_services.get<Renderer>();
                renderer->clear(true, true, false);
                m_systems.preRender(deltaTime);
                m_systems.render(deltaTime);
                window->swapBuffers();

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
    };

} // namespace engine