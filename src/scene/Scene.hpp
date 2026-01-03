#pragma once

#include "ecs/registry/Registry.hpp"

#include "render/Camera.hpp"

class Scene
{
private:
    Registry world;
    Camera mainCamera;

public:
    Scene()
    {
    }
    ~Scene()
    {
    }

    Registry &getWorld() { return world; }

    Camera &getMainCamera() { return mainCamera; }
};