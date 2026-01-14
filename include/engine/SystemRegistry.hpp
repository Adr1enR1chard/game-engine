#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "utils/types.hpp"
#include <engine/System.hpp>

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
    }

    /**
     * Remove systems from the registry.
     */
    template <SystemType... T> void remove()
    {
        (m_systems.erase(std::type_index(typeid(T))), ...);
    }

    void start(World& world, ServiceRegistry& services)
    {
        for (auto& [typeIndex, system] : m_systems) {
            system->start(world, services);
        }
    }

    void input(World& world, ServiceRegistry& services, float deltaTime)
    {
        for (auto& [typeIndex, system] : m_systems) {
            system->input(world, services, deltaTime);
        }
    }

    void update(World& world, ServiceRegistry& services, float deltaTime)
    {
        for (auto& [typeIndex, system] : m_systems) {
            system->update(world, services, deltaTime);
        }
    }

    void render(World& world, ServiceRegistry& services, float deltaTime)
    {
        for (auto& [typeIndex, system] : m_systems) {
            system->render(world, services, deltaTime);
        }
    }

  private:
    // TODO : Use vector instead of unordered_map for better cache coherence
    std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems;
};