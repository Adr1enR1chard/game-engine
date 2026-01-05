#pragma once
#include <GLFW/glfw3.h>

#include "engine/Service.hpp"

class Time : public Service
{

public:
    Time() = default;
    ~Time() = default;
    Time(const Time &) = delete;
    Time &operator=(const Time &) = delete;

    void update()
    {
        float currentFrameTime = static_cast<float>(glfwGetTime());
        m_deltaTime = currentFrameTime - m_lastFrameTime;
        m_lastFrameTime = currentFrameTime;
    }

    float deltaTime() const { return m_deltaTime; }

private:
    float m_deltaTime = 0.0f;
    float m_lastFrameTime = 0.0f;
};