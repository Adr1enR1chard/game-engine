#pragma once

#include "glad/glad.h"

namespace engine_editor
{
    class ViewportRenderer
    {
    public:
        static void Initialize();
        static void Resize(int width, int height);
        static void Begin();
        static void End();
        static GLuint GetFramebufferTexture();
        static GLuint GetFramebuffer();

        static int GetFramebufferWidth() { return m_width; }
        static int GetFramebufferHeight() { return m_height; }

    private:
        static GLuint m_framebufferTexture;
        static GLuint m_framebuffer;
        static GLuint m_renderbuffer;
        static int m_width;
        static int m_height;
    };
} // namespace engine_editor