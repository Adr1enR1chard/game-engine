#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
public:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        if (width == 0 || height == 0)
            return;

        glViewport(0, 0, width, height);

        window;
    }

private:
    GLFWwindow *m_window;

public:
    Window(int width, int height, const char *title)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // glfw window creation
        // --------------------
        GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }

        glEnable(GL_DEPTH_TEST);

        this->m_window = window;
    }
    ~Window()
    {
        glfwTerminate();
    }

    bool shouldClose() const
    {
        return glfwWindowShouldClose(this->m_window);
    }

    void makeContextCurrent()
    {
        glfwMakeContextCurrent(this->m_window);
    }

    void swapBuffers()
    {
        glfwSwapBuffers(this->m_window);
    }

    void pollEvents()
    {
        glfwPollEvents();
    }

    void clear()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};