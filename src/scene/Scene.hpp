#pragma once

#include "ecs/registry/Registry.hpp"
#include "ecs/system/SystemScheduler.hpp"
#include "render/Camera.hpp"

class Scene
{
private:
    Registry registry;
    Camera mainCamera;
    SystemScheduler systemScheduler;

public:
    Scene()
        : systemScheduler(*this)
    {
    }

    ~Scene()
    {
    }

    Registry &getRegistry() { return registry; }

    Camera &getMainCamera() { return mainCamera; }

    SystemScheduler &getSystemScheduler() { return systemScheduler; }
};