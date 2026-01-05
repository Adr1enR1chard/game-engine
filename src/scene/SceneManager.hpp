#pragma once
#include "scene/Scene.hpp"

class SceneManager
{
public:
    SceneManager() = default;
    ~SceneManager() = default;
    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;

    Scene &GetCurrentScene()
    {
        return m_CurrentScene;
    }

private:
    Scene m_CurrentScene;
};
