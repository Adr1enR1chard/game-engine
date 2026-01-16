#pragma once

#include <engine/model/Service.hpp>

#include <glm/glm.hpp>
#include <memory>

class Window : public Service
{
  public:
    Window();
    ~Window() override;

    void setResolution(int width, int height)
    {
        this->m_width  = width;
        this->m_height = height;
    }

    void setTitle(const char* title)
    {
        this->m_title = title;
    }

    void setFullscreen(bool fullscreen)
    {
        m_fullscreen = fullscreen;
    }

  private:
    friend class PlatformSystem;

    bool shouldClose() const;
    void create();
    void makeContextCurrent();
    void swapBuffers();
    void pollEvents();
    void clear();

  public:
    void getSize(int& width, int& height) const;
    void setClearColor(const glm::vec3& color)
    {
        m_clearColor = color;
    }

  private:
    struct WindowImpl;
    std::unique_ptr<WindowImpl> m_impl;
    glm::vec3                   m_clearColor = glm::vec3(0.0f, 0.0f, 0.0f);
    int                         m_width      = 800;
    int                         m_height     = 600;
    const char*                 m_title      = "Game Engine";
    bool                        m_fullscreen = false;
};