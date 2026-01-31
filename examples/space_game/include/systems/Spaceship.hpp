#pragma once

#include <DefaultBundle.hpp>

class Spaceship : public System
{
public:
    void start() override
    {
        m_spaceshipEntity = world().createEntity(
            CModelRenderer{
                .modelRef = services().get<ModelFactory>()->LoadModel("assets/models/spaceship/spaceship.gltf"),
            },
            CTransform{glm::vec3(0.0f, 0.0f, 0.0f)});
    }

    void update(double deltaTime) override
    {
        // Update spaceship logic
    }

private:
    Entity m_spaceshipEntity;
};