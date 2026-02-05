#include "EditorUI.hpp"
#include <string>

#include <component/CCamera.hpp>
#include <engine/registry/World.hpp>

#include <stdio.h>

namespace engine_editor
{
    Entity EditorUI::m_selectedEntity = 0;
    bool EditorUI::m_firstFrame = true;

    void EditorUI::Render(World &world, SystemRegistry &systems, engine::Renderer &renderer, int windowWidth, int windowHeight, float deltaTime)
    {
        // ----------------------------------
        // Render scene into framebuffer
        // ----------------------------------
        // sceneFB.Bind();
        // Renderer::Clear();
        // Renderer::RenderWorld(world, sceneCamera);
        // sceneFB.Unbind();

        // ================================
        // Editor Window
        // ================================
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
        ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        // Setup columns for entity list and scene viewport
        // the entity list will be on the left (25% width) and the scene viewport on the right (75% width)
        ImGui::Columns(2, nullptr, true);
        if (m_firstFrame)
        {
            ImGui::SetColumnWidth(0, windowWidth * 0.25f);
            m_firstFrame = false;
        }

        // ----------------
        // Entity List
        // ----------------
        ImGui::BeginChild("Entities", ImVec2(0, 0), true);

        world.forEachEntity(
            [](Entity e)
            {
                const std::string name = "Entity " + std::to_string(e); // world.GetEntityName(e);

                if (ImGui::Selectable(name.c_str(), false))
                {
                    m_selectedEntity = e;
                }
            });

        ImGui::EndChild();
        ImGui::NextColumn();

        // ----------------
        // Scene Viewport
        // ----------------
        ImGui::BeginChild(
            "Viewport",
            ImVec2(0, 0),
            true,
            ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoScrollWithMouse);

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();

        if (viewportSize.x > 0 && viewportSize.y > 0)
        {
            if (viewportSize.x != ViewportRenderer::GetFramebufferWidth() || viewportSize.y != ViewportRenderer::GetFramebufferHeight())
            {
                ViewportRenderer::Resize((int)viewportSize.x, (int)viewportSize.y);
            }

            ViewportRenderer::Begin();
            systems.preUpdate(deltaTime);
            systems.update(deltaTime);
            systems.preRender(deltaTime);
            systems.render(deltaTime);
            ViewportRenderer::End();

            ImGui::Image(
                (ImTextureID)ViewportRenderer::GetFramebufferTexture(),
                viewportSize,
                ImVec2(0, 1), // flip Y
                ImVec2(1, 0));

            // Route input only when hovered
            // if (ImGui::IsItemHovered())
            // {
            //     sceneCamera.HandleInput();
            // }
        }

        ImGui::EndChild();

        ImGui::Columns(1);
        ImGui::End();
    }
}