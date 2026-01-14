#include <engine/Bundle.hpp>

#include <engine/Engine.hpp>
#include <engine/system/CameraSystem.hpp>
// #include <engine/system/InputSystem.hpp>
#include <engine/Input.hpp>
#include <engine/Window.hpp>
#include <engine/system/LightSystem.hpp>
#include <engine/system/RenderSystem.hpp>
#include <engine/system/TransformSystem.hpp>

void DefaultBundle::apply(Engine& engine) const
{
    engine.addSystems<TransformSystem>();
    engine.addSystems<CameraSystem>();
    engine.addSystems<RenderSystem>();
    engine.addSystems<LightSystem>();
    // engine.add<InputSystem>();
    engine.addServices<Window>();
    engine.addServices<Input>();
}

void DefaultBundle::remove(Engine& engine) const
{
    engine.removeSystems<TransformSystem>();
    engine.removeSystems<CameraSystem>();
    engine.removeSystems<RenderSystem>();
    engine.removeSystems<LightSystem>();
    engine.removeServices<Window>();
    engine.removeServices<Input>();
}