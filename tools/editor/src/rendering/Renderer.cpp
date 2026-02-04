#include "Renderer.hpp"
#include <cstdio>

namespace engine_editor
{
    void Renderer::Initialize(int width, int height)
    {
        if (m_framebuffer != 0)
        {
            glDeleteFramebuffers(1, &m_framebuffer);
        }

        if (m_framebufferTexture != 0)
        {
            glDeleteTextures(1, &m_framebufferTexture);
        }

        glGenFramebuffers(1, &m_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        glGenTextures(1, &m_framebufferTexture);
        glBindTexture(GL_TEXTURE_2D, m_framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebufferTexture, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Error: Framebuffer is not complete!\n");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::Render()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::End()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint Renderer::GetFramebufferTexture()
    {
        return m_framebufferTexture;
    }

    GLuint Renderer::m_framebufferTexture = 0;
    GLuint Renderer::m_framebuffer = 0;
} // namespace engine_editor