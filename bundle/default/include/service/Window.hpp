#pragma once

#include <engine/model/WindowService.hpp>

#include <glm/glm.hpp>
#include <memory>

class Window : public WindowService
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