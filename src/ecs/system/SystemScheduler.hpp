#pragma once

#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <typeindex>
#include <iostream>

#include <engine/System.hpp>

#include "utils/types.hpp"

class EngineContext;

class SystemScheduler
{
public:
    SystemScheduler() {}
    ~SystemScheduler() {}

    template <SystemType T>
    bool registerSystem();

    template <SystemType T>
    bool unregisterSystem();

    void updateSystems(EngineContext &engineContext);

private:
    std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems;
};

template <SystemType T>
bool SystemScheduler::registerSystem()
{
    if (!m_systems.try_emplace(std::type_index(typeid(T)), std::make_unique<T>()).second)
    {
        std::cerr << "System already registered: " << typeid(T).name() << std::endl;
        return false;
    }
    return true;
}

template <SystemType T>
bool SystemScheduler::unregisterSystem()
{
    auto it = m_systems.find(std::type_index(typeid(T)));
    if (it == m_systems.end())
    {
        std::cerr << "SystemScheduler: System not found for deregistration: " << typeid(T).name() << std::endl;
        return false;
    }
    m_systems.erase(it);
    return true;
}