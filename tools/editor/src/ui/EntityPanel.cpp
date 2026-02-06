#include "EntityPanel.hpp"

#include <imgui.h>
#include <string>
#include <algorithm>

void EntityPanel::draw()
{
    ImGui::Begin("Entities");

    // Search bar
    ImGui::SetNextItemWidth(-1);
    ImGui::InputTextWithHint("##EntitySearch", "Search entities...", m_searchBuffer, sizeof(m_searchBuffer));
    ImGui::Spacing();

    // Entity list
    const float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild(
        "EntityList",
        ImVec2(0, -footerHeight),
        true,
        ImGuiWindowFlags_HorizontalScrollbar);

    std::string searchStr = m_searchBuffer;
    std::transform(searchStr.begin(), searchStr.end(), searchStr.begin(), ::tolower);
    bool hasEntities = false;

    world.forEachEntity([&](Entity e)
                        {
        hasEntities = true;
        std::string name = "Entity " + std::to_string(e);
        
        // Filter by search
        if (!searchStr.empty())
        {
            std::string lowerName = name;
            std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
            if (lowerName.find(searchStr) == std::string::npos)
            {
                return;
            }
        }

        const bool isSelected = (e == m_selectedEntity);
        
        // Selection styling
        if (isSelected)
        {
            ImGui::PushStyleColor(ImGuiCol_Header, ImGui::GetStyleColorVec4(ImGuiCol_HeaderActive));
        }

        if (ImGui::Selectable(name.c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick))
        {
            m_selectedEntity = e;
            
            // Double-click to focus (placeholder for future functionality)
            if (ImGui::IsMouseDoubleClicked(0))
            {
                // Could add focus/zoom to entity functionality here
            }
        }

        if (isSelected)
        {
            ImGui::PopStyleColor();
        }

        // Context menu
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete"))
            {
                m_entityToDelete = e;
                m_showDeleteConfirm = true;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Duplicate"))
            {
                // Placeholder for entity duplication
                world.createEntity();
            }
            ImGui::EndPopup();
        } });

    // Empty state
    if (!hasEntities)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
        ImGui::TextWrapped("No entities in the scene.\nClick 'Add Entity' to create one.");
        ImGui::PopStyleColor();
    }

    ImGui::EndChild();

    // Action buttons
    ImGui::Spacing();
    if (ImGui::Button("Add Entity", ImVec2(-1, 0)))
    {
        Entity newEntity = world.createEntity();
        m_selectedEntity = newEntity;
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("Create a new entity (Ctrl+N)");
    }

    // Delete confirmation popup
    if (m_showDeleteConfirm)
    {
        ImGui::OpenPopup("Delete Entity?");
        m_showDeleteConfirm = false;
    }

    if (ImGui::BeginPopupModal("Delete Entity?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Are you sure you want to delete Entity %d?", m_entityToDelete);
        ImGui::Text("This action cannot be undone.");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // if (ImGui::Button("Delete", ImVec2(120, 0)))
        // {
        //     world.destroyEntity(m_entityToDelete);
        //     if (m_selectedEntity == m_entityToDelete)
        //     {
        //         m_selectedEntity = 0;
        //     }
        //     ImGui::CloseCurrentPopup();
        // }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}