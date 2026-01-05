#pragma once
#include <GLFW/glfw3.h>

class Time
{

public:
    Time() = default;
    ~Time() = default;
    Time(const Time &) = delete;
    Time &operator=(const Time &) = delete;

    void update()
    {
        float currentFrameTime = static_cast<float>(glfwGetTime());
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
    }

    float getDeltaTime() const { return deltaTime; }

private:
    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;
};