#pragma once

#include <imgui.h>

class ViewportPanel
{
public:
    ViewportPanel() = default;
    ~ViewportPanel() = default;

    void draw(ImTextureRef framebufferTexture);

    ImVec2 getViewportSize() const
    {
        return m_viewportSize;
    }

private:
    ImVec2 m_viewportSize = ImVec2(0, 0);
};