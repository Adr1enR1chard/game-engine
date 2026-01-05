#include "ecs/system/SystemScheduler.hpp"

#include "SystemScheduler.hpp"

#include <engine/EngineContext.hpp>

void SystemScheduler::updateSystems(EngineContext &engineContext)
{
    for (auto &[typeIndex, system] : m_systems)
    {
        system->update(engineContext);
    }
}
