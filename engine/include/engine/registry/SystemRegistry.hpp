#pragma once

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <engine/model/System.hpp>
#include <engine/registry/SystemStorage.hpp>

#include <engine/utils/Log.hpp>

namespace engine
{

    class World;
    class ServiceRegistry;

    class SystemRegistry
    {
    public:
        SystemRegistry() = default;
        ~SystemRegistry() = default;
        SystemRegistry(const SystemRegistry &) = delete;
        SystemRegistry &operator=(const SystemRegistry &) = delete;

        /**
         * Add systems to the registry.
         */
        template <SystemType... T>
        void add()
        {
            (m_systems.emplace<T>(), ...);
            (..., Log::Print("Added system: " + std::string(typeid(T).name()), LogLevel::Info));
        }

        template <SystemType T, typename... Args>
        void add(Args &&...args)
        {
            m_systems.emplace<T>(std::forward<Args>(args)...);
            Log::Print("Added system: " + std::string(typeid(T).name()), LogLevel::Info);
        }

        /**
         * Remove systems from the registry.
         */
        template <SystemType... T>
        void remove()
        {
            (m_systems.erase<T>(), ...);
        }

        void setContext(World &world, ServiceRegistry &services)
        {
            m_systems.map([&](System &sys)
                          { sys.setContext(world, services); });
        }

        void init()
        {
            m_systems.map([&](System &system)
                          { system.init(); });
        }

        void start()
        {
            m_systems.map([&](System &system)
                          { system.start(); });
        }

        void update(float deltaTime)
        {
            m_systems.map([&](System &system)
                          { system.update(deltaTime); });
        }

    private:
        SystemStorage m_systems;
    };

} // namespace engine