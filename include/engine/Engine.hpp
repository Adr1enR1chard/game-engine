#pragma once
#include "engine/SceneManager.hpp"
#include "engine/Window.hpp"
#include "engine/Time.hpp"
#include "engine/EngineContext.hpp"

class Engine
{
public:
    Engine(int width, int height, const char *title)
    {
        engineContext.registerService<SceneManager>(std::make_unique<SceneManager>());
        engineContext.registerService<Window>(std::make_unique<Window>(width, height, title));
        engineContext.registerService<Time>(std::make_unique<Time>());
    }
    ~Engine() = default;

    void run()
    {
        Window &window = engineContext.getService<Window>();
        Time &time = engineContext.getService<Time>();
        SceneManager &sceneManager = engineContext.getService<SceneManager>();

        while (!window.shouldClose())
        {
            window.makeContextCurrent();
            window.clear();

            // Update current scene
            Scene &currentScene = sceneManager.currentScene();
            currentScene.systems().updateSystems(engineContext);

            window.swapBuffers();
            window.pollEvents();
            time.update();
        }
    }

    EngineContext &context()
    {
        return engineContext;
    }

private:
    EngineContext engineContext;
};