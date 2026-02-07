#pragma once

#include <imgui.h>

#include "rendering/Renderer.hpp"
#include <engine/registry/World.hpp>
#include <engine/registry/SystemRegistry.hpp>
#include <engine/model/Entity.hpp>
#include <engine/bundle/core/service/Renderer.hpp>

using namespace engine;

namespace engine_editor
{

    class EditorUI
    {
    public:
        static void Render(World &world, SystemRegistry &systems, ServiceRegistry &services, int windowWidth, int windowHeight, float deltaTime);

    private:
        static bool m_firstFrame;
        static Entity m_selectedEntity;
    };
} // namespace engine_editor