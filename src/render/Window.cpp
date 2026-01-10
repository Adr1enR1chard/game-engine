#include <engine/Window.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (width == 0 || height == 0)
        return;

    glViewport(0, 0, width, height);

    window;
}

Window::~Window()
{
    glfwTerminate();
}

void Window::init(int width, int height, const char* title)
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
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glfwSwapInterval(0); // Disable V-Sync
    glEnable(GL_DEPTH_TEST);

    this->m_window = window;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(this->m_window);
}

void Window::makeContextCurrent()
{
    glfwMakeContextCurrent(this->m_window);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(this->m_window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::clear()
{
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::getSize(int& width, int& height) const
{
    glfwGetFramebufferSize(this->m_window, &width, &height);
}
