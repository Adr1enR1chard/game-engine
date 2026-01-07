#pragma once
#include <chrono>
#include <thread>

#include <engine/SceneManager.hpp>
#include <engine/Window.hpp>
#include <engine/EngineContext.hpp>

class Engine
{
public:
    Engine(int width, int height, const char *title)
    {
        engineContext.registerService<SceneManager>(std::make_unique<SceneManager>());
        engineContext.registerService<Window>(std::make_unique<Window>(width, height, title));
    }
    ~Engine() = default;

    void run()
    {
        Window &window = engineContext.getService<Window>();
        SceneManager &sceneManager = engineContext.getService<SceneManager>();

        using clock = std::chrono::steady_clock;
        using frames = std::chrono::duration<int, std::ratio<1, 144>>;

        auto lastFrame = clock::now();

        while (!window.shouldClose())
        {
            auto frameStart = clock::now();
            std::chrono::duration<double> delta = frameStart - lastFrame;
            lastFrame = frameStart;

            // --- Engine loop ---
            window.clear();

            Scene &currentScene = sceneManager.currentScene();
            currentScene.systems().updateSystems(engineContext, delta.count());

            window.swapBuffers();
            window.pollEvents();
        }
    }

    EngineContext &context()
    {
        return engineContext;
    }

private:
    EngineContext engineContext;
};