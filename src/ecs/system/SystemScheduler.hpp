#pragma once

#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <stdexcept>
#include <typeindex>

#include "utils/types.hpp"

#include "ecs/system/System.hpp"

class Scene;

class SystemScheduler
{
public:
    SystemScheduler(Scene &scene) : m_scene(scene) {}
    ~SystemScheduler() {}

    template <SystemType System>
    void registerSystem();

    template <SystemType System>
    void unregisterSystem();

    void updateSystems(float deltaTime);

private:
    Scene &m_scene;

    std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems;
};

template <SystemType System>
void SystemScheduler::registerSystem()
{
    if (!m_systems.try_emplace(std::type_index(typeid(System)), std::make_unique<System>()).second)
    {
        throw std::runtime_error("System already registered.");
    }
}

template <SystemType System>
void SystemScheduler::unregisterSystem()
{
    auto it = m_systems.find(std::type_index(typeid(System)));
    if (it == m_systems.end())
    {
        throw std::runtime_error("System not registered.");
    }
    m_systems.erase(it);
}