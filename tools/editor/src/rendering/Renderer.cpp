#include "Renderer.hpp"
#include <cstdio>

#include <stdexcept>

namespace engine_editor
{
    int ViewportRenderer::m_width = 0;
    int ViewportRenderer::m_height = 0;

    void ViewportRenderer::Initialize()
    {
        printf("Editor::ViewportRenderer::Resize - Resizing viewport to %d x %d\n", m_width, m_height);
        glGenFramebuffers(1, &m_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        glGenTextures(1, &m_framebufferTexture);
        glBindTexture(GL_TEXTURE_2D, m_framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebufferTexture, 0);

        glGenRenderbuffers(1, &m_renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            printf("Error creating framebuffer for viewport renderer: 0x%X\n", err);
        }
    }

    void ViewportRenderer::Resize(int width, int height)
    {
        if (m_framebuffer == 0)
        {
            m_width = width;
            m_height = height;
            Initialize();
        }

        glBindTexture(GL_TEXTURE_2D, m_framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_framebufferTexture, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        m_width = width;
        m_height = height;
    }

    void ViewportRenderer::Begin()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            printf("Error setting up viewport framebuffer for rendering: 0x%X\n", err);
        }

        glViewport(0, 0, m_width, m_height);
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void ViewportRenderer::End()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            printf("Error unbinding viewport framebuffer after rendering: 0x%X\n", err);
        }
    }

    GLuint ViewportRenderer::GetFramebufferTexture()
    {
        return m_framebufferTexture;
    }

    GLuint ViewportRenderer::GetFramebuffer()
    {
        return m_framebuffer;
    }

    GLuint ViewportRenderer::m_framebufferTexture = 0;
    GLuint ViewportRenderer::m_framebuffer = 0;
    GLuint ViewportRenderer::m_renderbuffer = 0;
} // namespace engine_editor