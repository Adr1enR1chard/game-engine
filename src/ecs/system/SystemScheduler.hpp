#pragma once

#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <typeindex>
#include <iostream>

#include "utils/types.hpp"

#include "ecs/system/System.hpp"

class EngineContext;

class SystemScheduler
{
public:
    SystemScheduler() {}
    ~SystemScheduler() {}

    template <SystemType System>
    bool registerSystem();

    template <SystemType System>
    bool unregisterSystem();

    void updateSystems(EngineContext &engineContext);

private:
    std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems;
};

template <SystemType System>
bool SystemScheduler::registerSystem()
{
    if (!m_systems.try_emplace(std::type_index(typeid(System)), std::make_unique<System>()).second)
    {
        std::cerr << "System already registered: " << typeid(System).name() << std::endl;
        return false;
    }
    return true;
}

template <SystemType System>
bool SystemScheduler::unregisterSystem()
{
    auto it = m_systems.find(std::type_index(typeid(System)));
    if (it == m_systems.end())
    {
        std::cerr << "SystemScheduler: System not found for deregistration: " << typeid(System).name() << std::endl;
        return false;
    }
    m_systems.erase(it);
    return true;
}