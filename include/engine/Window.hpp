#pragma once

#include <glm/glm.hpp>

#include "engine/Service.hpp"

class Window : public Service
{
  private:
    static void framebuffer_size_callback(struct GLFWwindow* window, int width, int height);

  public:
    Window() = default;
    ~Window();

  private:
    void init(int width, int height, const char* title, bool fullscreen);
    bool shouldClose() const;
    void makeContextCurrent();
    void swapBuffers();
    void pollEvents();
    void clear();

    friend class Engine;

  public:
    void getSize(int& width, int& height) const;
    void setClearColor(const glm::vec3& color)
    {
        m_clearColor = color;
    }

  private:
    struct GLFWwindow* m_window;
    glm::vec3          m_clearColor = glm::vec3(0.0f, 0.0f, 0.0f);
};