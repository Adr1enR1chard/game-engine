#pragma once
#include "scene/Scene.hpp"

class SceneManager
{
public:
    static SceneManager &Instance()
    {
        static SceneManager instance;
        return instance;
    }

    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;

    static Scene &GetCurrentScene()
    {
        return Instance().m_CurrentScene;
    }

private:
    SceneManager() = default;
    ~SceneManager() = default;

    Scene m_CurrentScene;
};
