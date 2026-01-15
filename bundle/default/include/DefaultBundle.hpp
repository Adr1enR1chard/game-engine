#pragma once

#include <engine/Engine.hpp>

// --- Services ---
#include <service/Input.hpp>
#include <service/Window.hpp>

// --- Systems ---
#include <system/CameraSystem.hpp>
#include <system/LightSystem.hpp>
#include <system/RenderSystem.hpp>
#include <system/TransformSystem.hpp>

// --- Components ---
#include <component/CCamera.hpp>
#include <component/CDirectionalLight.hpp>
#include <component/CMeshRenderer.hpp>
#include <component/CPointLight.hpp>
#include <component/CTransform.hpp>

class DefaultBundle : public Bundle
{
  public:
    void apply(Engine& engine) const override
    {
        engine.addSystems<TransformSystem>();
        engine.addSystems<CameraSystem>();
        engine.addSystems<RenderSystem>();
        engine.addSystems<LightSystem>();
        // engine.add<InputSystem>();
        engine.addServices<Window>();
        engine.addServices<Input>();
    }
    void remove(Engine& engine) const override
    {
        engine.removeSystems<TransformSystem>();
        engine.removeSystems<CameraSystem>();
        engine.removeSystems<RenderSystem>();
        engine.removeSystems<LightSystem>();
        engine.removeServices<Window>();
        engine.removeServices<Input>();
    }
};