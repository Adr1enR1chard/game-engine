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
        static Engine &InitializeStandalone();
        static Engine &InitializeEmbedded(World *&outWorld, SystemRegistry *&outSystems, ServiceRegistry *&outServices);
        static void Shutdown();
        static void ExecuteOnMainThread(const std::function<void()> &task);

    private:
        Engine();

        static std::unique_ptr<Engine> m_instance;

    public:
        Engine(const Engine &) = delete;
        Engine &operator=(const Engine &) = delete;

        void run(unsigned int width, unsigned int height, const char *title, bool fullscreen = false);

        /**
         * Use a bundle to add a set of systems to the engine.
         */
        template <BundleType B>
        Engine &addBundle()
        {
            std::unique_ptr<Bundle> bundle = std::make_unique<B>();
            for (const std::type_index &requiredBundleType : bundle->getRequiredBundles())
            {
                bool found = false;
                for (const auto &existingBundle : m_bundles)
                {
                    if (std::type_index(typeid(*existingBundle)) == requiredBundleType)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    Log::Print("Bundle " + std::string(typeid(B).name()) + " requires bundle " + std::string(requiredBundleType.name()) + " to be installed.", LogLevel::Critical);
                    throw std::runtime_error("Bundle dependency not met.");
                }
            }
            bundle->install(*this);

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
                (*it)->uninstall(*this);
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
            m_systems.add<Ts...>(m_world, m_services);

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

        template <ServiceType T, typename... Args>
        Engine &addService(Args &&...args)
        {
            m_services.add<T>(std::forward<Args>(args)...);
            return *this;
        }

        template <ServiceType T>
        T *getService()
        {
            return m_services.get<T>();
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