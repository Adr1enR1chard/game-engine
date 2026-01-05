#pragma once
#include "scene/SceneManager.hpp"
#include "engine/Window.hpp"
#include "engine/Time.hpp"

class Engine
{
public:
    Engine(int width, int height, const char *title)
        : sceneManager(), window(width, height, title), time()
    {
    }
    ~Engine() = default;

    void createWindow(int width, int height, const char *title)
    {
        this->window = Window(width, height, title);
    }

    void run()
    {
        try
        {
            while (!this->window.shouldClose())
            {
                this->window.makeContextCurrent();
                this->window.clear();

                // Update current scene
                Scene &currentScene = this->sceneManager.GetCurrentScene();
                currentScene.getSystemScheduler().updateSystems(this->time.getDeltaTime());

                this->window.swapBuffers();
                this->window.pollEvents();

                this->time.update();
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Engine run-time error: " << e.what() << std::endl;
        }
    }

    SceneManager &getSceneManager()
    {
        return this->sceneManager;
    }

    Window &getWindow()
    {
        return this->window;
    }

    Time &getTime()
    {
        return this->time;
    }

private:
    SceneManager sceneManager;
    Window window;
    Time time;
};