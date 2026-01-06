#pragma once

#include "engine/Service.hpp"

class Window : public Service
{
private:
    static void framebuffer_size_callback(struct GLFWwindow *window, int width, int height);

public:
    Window(int width, int height, const char *title);
    ~Window();

private:
    bool shouldClose() const;
    void makeContextCurrent();
    void swapBuffers();
    void pollEvents();
    void clear();

    friend class Engine;

public:
    void getSize(int &width, int &height) const;

private:
    struct GLFWwindow *m_window;
};