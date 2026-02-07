#include "EditorUI.hpp"
#include <string>

#include <RasterizationBundle.hpp>
#include <engine/registry/World.hpp>

#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

using namespace rasterization_bundle;

namespace engine_editor
{
    Entity EditorUI::m_selectedEntity = 0;
    bool EditorUI::m_firstFrame = true;

    void EditorUI::Render(World &world, SystemRegistry &systems, ServiceRegistry &services, int windowWidth, int windowHeight, float deltaTime)
    {
        // ================================
        // Editor Window
        // ================================
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);

        ImGui::Begin(
            "Editor",
            nullptr,
            ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove);

        // -------------------------------------------------
        // Layout sizes
        // -------------------------------------------------
        const float leftPanelWidth = windowWidth * 0.10f;     // 10%
        const float viewportPanelWidth = windowWidth * 0.70f; // 70%
        const float rightPanelWidth = windowWidth * 0.20f;    // 20%

        // =================================================
        // LEFT PANEL (Entities + Inspector)
        // =================================================
        ImGui::BeginChild(
            "LeftPanel",
            ImVec2(leftPanelWidth, 0),
            true);

        // ----------------
        // Entity List
        // ----------------

        ImGui::EndChild();

        // =================================================
        // (Viewport)
        // =================================================
        ImGui::SameLine();

        // ----------------
        // Component Inspector
        // ----------------
        ImGui::SameLine();

        ImGui::BeginChild(
            "Components",
            ImVec2(0, 0),
            true);

        ImGui::SeparatorText("Inspector");

        if (m_selectedEntity != 0)
        {
            ImGui::Text("Entity: %d", m_selectedEntity);
            ImGui::Separator();

            if (world.hasComponents<CTransform>(m_selectedEntity))
            {
                auto [entity, transform] = world.fetchFrom<CTransform>(m_selectedEntity);
                if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::DragFloat3("Position", glm::value_ptr(transform->position), 0.1f);

                    glm::vec3 euler = glm::degrees(glm::eulerAngles(transform->rotation));
                    if (ImGui::DragFloat3("Rotation", glm::value_ptr(euler), 0.5f))
                    {
                        transform->setRotationFromEuler(glm::radians(euler));
                    }

                    ImGui::DragFloat3("Scale", glm::value_ptr(transform->scale), 0.1f);
                    if (transform->scale.x < 0.001f)
                        transform->scale.x = 0.001f;
                    if (transform->scale.y < 0.001f)
                        transform->scale.y = 0.001f;
                    if (transform->scale.z < 0.001f)
                        transform->scale.z = 0.001f;
                }
            }

            if (world.hasComponents<CDirectionalLight>(m_selectedEntity))
            {
                auto [entity, light] = world.fetchFrom<CDirectionalLight>(m_selectedEntity);
                if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    // Direction
                    ImGui::DragFloat3("Direction", glm::value_ptr(light->direction), 0.1f, -1.0f, 1.0f);
                    ImGui::ColorEdit3("Color", glm::value_ptr(light->color));
                    ImGui::DragFloat("Ambient", &light->ambient, 0.1f, 0.0f);
                    ImGui::DragFloat("Intensity", &light->intensity, 0.1f, 0.0f);
                }
            }

            if (world.hasComponents<CCamera>(m_selectedEntity))
            {
                auto [entity, camera] = world.fetchFrom<CCamera>(m_selectedEntity);
                if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::DragFloat("FOV", &camera->fov, 0.1f, 1.0f, 179.0f);
                    ImGui::DragFloat("Near Plane", &camera->nearPlane, 0.01f, 0.01f);
                    ImGui::DragFloat("Far Plane", &camera->farPlane, 1.0f, 1.0f);
                    ImGui::Checkbox("Active", &camera->isActive);
                }
            }
        }
        else
        {
            ImGui::TextDisabled("No entity selected");
        }

        ImGui::EndChild();

        ImGui::End();
    }
}