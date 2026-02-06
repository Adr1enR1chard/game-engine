#include "ViewportPanel.hpp"

void ViewportPanel::draw(ImTextureRef framebufferTexture)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PopStyleVar();

    m_viewportSize = ImGui::GetContentRegionAvail();

    if (m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f)
    {
        // // TODO: Only do this on resize
        // ViewportRenderer::Resize(
        //     (int)viewportSize.x,
        //     (int)viewportSize.y);

        // ViewportRenderer::Begin();
        // systems.update(deltaTime);
        // ViewportRenderer::End();

        ImGui::Image(
            framebufferTexture,
            m_viewportSize,
            ImVec2(0, 1),
            ImVec2(1, 0));
    }

    ImGui::End();
}