#pragma once

class Scene;

class System
{
public:
    virtual ~System() = default;
    virtual void update(Scene &scene, float deltaTime) = 0;
};