#pragma once

#include <DefaultRenderingBundle.hpp>

using namespace engine;
using namespace default_rendering;

class DefaultWorld : public System
{
public:
    void start() override
    {
        world().createEntity(CCamera{}, CTransform{.position = glm::vec3(0.0f, 0.0f, 5.0f)});

        world().createEntity(CEnvironment{.skyboxMaterial = services().get<MaterialFactory>()->SkyboxMaterial()});

        world().createEntity(CDirectionalLight{.direction = glm::vec3(-0.2f, -1.0f, -0.3f), .color = glm::vec3(1.0f), .ambient = 1.0f, .intensity = 10.0f});
    }

    void update(float deltaTime) override
    {
    }

private:
    float m_rotationAngle = 0.0f;
};