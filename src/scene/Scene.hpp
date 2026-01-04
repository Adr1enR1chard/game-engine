#pragma once

#include "ecs/registry/Registry.hpp"

#include "render/Camera.hpp"

class Scene
{
private:
    Registry registry;
    Camera mainCamera;

public:
    Scene()
    {
    }
    ~Scene()
    {
    }

    Registry &getRegistry() { return registry; }

    Camera &getMainCamera() { return mainCamera; }
};