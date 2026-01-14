#include <engine/Input.hpp>
#include <engine/Window.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include "render/KeyMap.cpp"

struct Window::WindowImpl {
    struct GLFWwindow* m_window;

    static void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height)
    {
        if (width == 0 || height == 0)
            return;

        glViewport(0, 0, width, height);
    }
};

Window::Window() : m_impl(std::make_unique<WindowImpl>()) {}

Window::~Window()
{
    glfwTerminate();
}

void Window::init(int width, int height, const char* title, bool fullscreen)
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
    GLFWwindow* window = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Window::WindowImpl::framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glfwSetKeyCallback(window, [](GLFWwindow* /*wnd*/, int key, int scancode, int action, int mods) {
        Input::KeyCallback(GLFWKeyToEngineKey(key), GLFWKeyToEngineKey(scancode), GLFWActionToEngineAction(action),
                           mods);
    });
    glfwSetCursorPosCallback(
        window, [](GLFWwindow* /*wnd*/, double xpos, double ypos) { Input::MousePositionCallback(xpos, ypos); });

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSwapInterval(1); // Enable vsync
    glEnable(GL_DEPTH_TEST);

    m_impl->m_window = window;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_impl->m_window);
}

void Window::makeContextCurrent()
{
    glfwMakeContextCurrent(m_impl->m_window);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(m_impl->m_window);
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
    glfwGetFramebufferSize(m_impl->m_window, &width, &height);
}
