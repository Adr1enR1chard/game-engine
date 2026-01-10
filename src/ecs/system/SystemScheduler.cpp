#include "ecs/system/SystemScheduler.hpp"

#include "SystemScheduler.hpp"

#include <engine/World.hpp>

#include <iostream>

void SystemScheduler::updateSystems(World& world, double deltaTime)
{
    for (auto& [typeIndex, system] : m_systems) {
        system->update(world, deltaTime);
    }
}
