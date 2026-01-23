#pragma once

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <engine/model/System.hpp>

#include <engine/utils/Log.hpp>

class World;
class ServiceRegistry;

class SystemRegistry
{
  public:
    SystemRegistry()                                 = default;
    ~SystemRegistry()                                = default;
    SystemRegistry(const SystemRegistry&)            = delete;
    SystemRegistry& operator=(const SystemRegistry&) = delete;

    /**
     * Add systems to the registry.
     */
    template <SystemType... T> void add()
    {
        (m_systems.try_emplace(std::type_index(typeid(T)), std::make_unique<T>()), ...);
        (..., Log::Print("Added system: " + std::string(typeid(T).name()), LogLevel::Info));
    }

    template <SystemType T, typename... Args> void add(Args&&... args)
    {
        m_systems.try_emplace(std::type_index(typeid(T)), std::make_unique<T>(std::forward<Args>(args)...));
        Log::Print("Added system: " + std::string(typeid(T).name()), LogLevel::Info);
    }

    /**
     * Remove systems from the registry.
     */
    template <SystemType... T> void remove()
    {
        (m_systems.erase(std::type_index(typeid(T))), ...);
    }

    void setContext(World& world, ServiceRegistry& services)
    {
        for (auto& [typeIndex, system] : m_systems) {
            system->setContext(world, services);
        }
    }

    void init()
    {
        for (auto& [typeIndex, system] : m_systems) {
            system->init();
        }
    }

    void start()
    {
        for (auto& [typeIndex, system] : m_systems) {
            system->start();
        }
    }

    void preUpdate(float deltaTime)
    {
        for (auto& [typeIndex, system] : m_systems) {
            system->preUpdate(deltaTime);
        }
    }

    void update(float deltaTime)
    {
        for (auto& [typeIndex, system] : m_systems) {
            system->update(deltaTime);
        }
    }

    void preRender(float deltaTime)
    {
        for (auto& [typeIndex, system] : m_systems) {
            system->preRender(deltaTime);
        }
    }

    void render(float deltaTime)
    {
        for (auto& [typeIndex, system] : m_systems) {
            system->render(deltaTime);
        }
    }

    void present(float deltaTime)
    {
        for (auto& [typeIndex, system] : m_systems) {
            system->present(deltaTime);
        }
    }

  private:
    // TODO : Use vector instead of unordered_map for better cache coherence
    std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems;
};