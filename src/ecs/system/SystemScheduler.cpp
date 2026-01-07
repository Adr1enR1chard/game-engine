#include "ecs/system/SystemScheduler.hpp"

#include "SystemScheduler.hpp"

#include <engine/EngineContext.hpp>

#include <iostream>

void SystemScheduler::updateSystems(EngineContext &engineContext, double deltaTime)
{
    for (auto &[typeIndex, system] : m_systems)
    {
        system->update(engineContext, deltaTime);
    }
}
