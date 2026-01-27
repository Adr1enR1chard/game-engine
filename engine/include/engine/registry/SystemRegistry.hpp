#pragma once

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <engine/model/System.hpp>
#include <engine/registry/SystemStorage.hpp>

#include <engine/utils/Log.hpp>

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

private:
    friend class Engine;
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

    void preUpdate(float deltaTime)
    {
        m_systems.map([&](System &system)
                      { system.preUpdate(deltaTime); });
    }

    void update(float deltaTime)
    {
        m_systems.map([&](System &system)
                      { system.update(deltaTime); });
    }

    void preRender(float deltaTime)
    {
        m_systems.map([&](System &system)
                      { system.preRender(deltaTime); });
    }

    void render(float deltaTime)
    {
        m_systems.map([&](System &system)
                      { system.render(deltaTime); });
    }

    void present(float deltaTime)
    {
        m_systems.map([&](System &system)
                      { system.present(deltaTime); });
    }

private:
    SystemStorage m_systems;
};