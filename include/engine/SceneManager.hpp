#pragma once
#include "scene/Scene.hpp"

#include "engine/Service.hpp"

class SceneManager : public Service
{
public:
    SceneManager() = default;
    virtual ~SceneManager() = default;
    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;

    Scene &currentScene()
    {
        return m_CurrentScene;
    }

private:
    Scene m_CurrentScene;
};
