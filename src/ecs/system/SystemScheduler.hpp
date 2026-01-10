#pragma once

#include <iostream>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <engine/System.hpp>

#include "utils/types.hpp"

enum class SystemPhase {
    Update,
    Start,
};

class SystemScheduler
{
  public:
    SystemScheduler() {}
    ~SystemScheduler() {}

    template <SystemType T> bool registerSystem(SystemPhase phase = SystemPhase::Update);

    template <SystemType T> bool unregisterSystem();

    void startSystems(World& world);
    void updateSystems(World& world, double deltaTime);

  private:
    std::unordered_map<std::type_index, std::unique_ptr<System>> m_updateSystems;
    std::unordered_map<std::type_index, std::unique_ptr<System>> m_startSystems;
};

template <SystemType T> bool SystemScheduler::registerSystem(SystemPhase phase)
{
    if (phase == SystemPhase::Update) {
        if (!m_updateSystems.try_emplace(std::type_index(typeid(T)), std::make_unique<T>()).second) {
            std::cerr << "System already registered: " << typeid(T).name() << std::endl;
            return false;
        }
    } else if (phase == SystemPhase::Start) {
        if (!m_startSystems.try_emplace(std::type_index(typeid(T)), std::make_unique<T>()).second) {
            std::cerr << "System already registered: " << typeid(T).name() << std::endl;
            return false;
        }
    } else {
        std::cerr << "Unknown system phase for registration: " << static_cast<int>(phase) << std::endl;
        return false;
    }
    return true;
}

template <SystemType T> bool SystemScheduler::unregisterSystem()
{
    auto it = m_startSystems.find(std::type_index(typeid(T)));
    if (it != m_startSystems.end()) {
        m_startSystems.erase(it);
        return true;
    }

    it = m_updateSystems.find(std::type_index(typeid(T)));
    if (it != m_updateSystems.end()) {
        m_updateSystems.erase(it);
        return true;
    }

    return false;
}