#include <engine/Bundle.hpp>

#include <ecs/system/SystemScheduler.hpp>
#include <engine/system/CameraSystem.hpp>
#include <engine/system/LightSystem.hpp>
#include <engine/system/RenderSystem.hpp>
#include <engine/system/TransformSystem.hpp>

void DefaultBundle::apply(SystemScheduler& scheduler) const
{
    scheduler.registerSystem<TransformSystem>();
    scheduler.registerSystem<CameraSystem>();
    scheduler.registerSystem<RenderSystem>();
    scheduler.registerSystem<LightSystem>();
}