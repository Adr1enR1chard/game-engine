#pragma once

#include "ecs/Registry.hpp"
#include "ecs/system/SystemScheduler.hpp"

class Scene
{
private:
    Registry m_registry;
    SystemScheduler m_systemScheduler;

public:
    Scene()
    {
    }

    ~Scene()
    {
    }

    Registry &registry() { return m_registry; }

    SystemScheduler &systems() { return m_systemScheduler; }
};