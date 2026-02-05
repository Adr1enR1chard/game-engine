#pragma once

#include <DefaultBundle.hpp>

using namespace engine;
using namespace default_bundle;

class DefaultWorld : public System
{
public:
    void start() override
    {
        world().createEntity(CCamera{}, CTransform{.position = glm::vec3(0.0f, 0.0f, 5.0f)});

        world().createEntity(CEnvironment{.skyboxMaterial = services().get<MaterialFactory>()->SkyboxMaterial()}, CTransform{});

        world().createEntity(CMeshRenderer{.meshRef = services().get<MeshFactory>()->Cube(), .material = services().get<MaterialFactory>()->PBRMaterial()}, CTransform{});

        world().createEntity(CDirectionalLight{.direction = glm::vec3(-0.2f, -1.0f, -0.3f), .color = glm::vec3(1.0f), .ambient = 1.0f, .intensity = 10.0f}, CTransform{});
    }

    void update(float deltaTime) override
    {
        m_rotationAngle += deltaTime * 0.5f; // Rotate at 0.5 radians per second
        auto entities = world().fetch<CCamera, CTransform>();
        for (const auto &[entity, cameraCache, transformCache] : entities)
        {
            transformCache->setRotationFromEuler(glm::vec3(0.0f, m_rotationAngle, 0.0f));
        }
    }

private:
    float m_rotationAngle = 0.0f;
};