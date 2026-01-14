#pragma once

#include <memory>

#include "engine/IWindow.hpp"
#include <glm/glm.hpp>

#include "engine/Service.hpp"

class Window : public Service, public IWindow
{
  public:
    Window();
    ~Window();

    void init(int width, int height, const char* title, bool fullscreen);

  private:
    bool shouldClose() const;
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
};