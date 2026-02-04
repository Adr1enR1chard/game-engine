#pragma once

#include "glad/glad.h"

namespace engine_editor
{
    class Renderer
    {
    public:
        static void Initialize(int width, int height);
        static void Render();
        static void End();
        static GLuint GetFramebufferTexture();

    private:
        static GLuint m_framebufferTexture;
        static GLuint m_framebuffer;
    };
} // namespace engine_editor