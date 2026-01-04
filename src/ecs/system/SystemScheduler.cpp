#include "ecs/system/SystemScheduler.hpp"

#include "scene/Scene.hpp"
#include "SystemScheduler.hpp"

void SystemScheduler::updateSystems(float deltaTime)
{
    for (auto &[typeIndex, system] : m_systems)
    {
        system->update(m_scene, deltaTime);
    }
}
