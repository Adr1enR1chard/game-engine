#include <engine/bundle/standalone/service/Input.hpp>
#include <engine/bundle/standalone/service/Window.hpp>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include <engine/Engine.hpp>
#include <engine/utils/key_mapping.hpp>

namespace engine
{

    struct Window::WindowImpl
    {
        struct GLFWwindow *m_window;

        static void framebuffer_size_callback(GLFWwindow * /*window*/, int width, int height)
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

    void Window::create()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // glfw window creation
        // --------------------
        GLFWwindow *window =
            glfwCreateWindow(m_width, m_height, m_title, m_fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }
        m_impl->m_window = window;

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, Window::WindowImpl::framebuffer_size_callback);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return;
        }

        glfwSetKeyCallback(window, [](GLFWwindow * /*wnd*/, int key, int scancode, int action, int mods)
                           { Input::KeyCallback(GLFWKeyToEngineKey(key), GLFWKeyToEngineKey(scancode), GLFWActionToEngineAction(action),
                                                mods); });
        glfwSetCursorPosCallback(
            window, [](GLFWwindow * /*wnd*/, double xpos, double ypos)
            { Input::MousePositionCallback(xpos, ypos); });
        glfwSetWindowCloseCallback(window, [](GLFWwindow * /*wnd*/)
                                   { Engine::Shutdown(); });

        captureMouse(true);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        // Enable vsync
        glfwSwapInterval(1);
    }

    void Window::setSize(int width, int height)
    {
        m_width = width;
        m_height = height;
        glfwSetWindowSize(m_impl->m_window, width, height);
    }

    void Window::captureMouse(bool capture)
    {
        if (capture)
        {
            glfwSetInputMode(m_impl->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            glfwSetInputMode(m_impl->m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
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

    void Window::getSize(int &width, int &height) const
    {
        glfwGetFramebufferSize(m_impl->m_window, &width, &height);
    }

} // namespace engine
