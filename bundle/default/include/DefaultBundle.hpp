#pragma once

#include <engine/Engine.hpp>

// --- Services ---
#include <service/Input.hpp>
#include <service/Window.hpp>

// --- Systems ---
#include <system/CameraSystem.hpp>
#include <system/LightSystem.hpp>
#include <system/PlatformSystem.hpp>
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
        engine.addSystems<TransformSystem, CameraSystem, RenderSystem, LightSystem, PlatformSystem>();
        engine.addServices<Window, Input>();
    }
    void remove(Engine& engine) const override
    {
        engine.removeSystems<TransformSystem, CameraSystem, RenderSystem, LightSystem, PlatformSystem>();
        engine.removeServices<Window, Input>();
    }
};