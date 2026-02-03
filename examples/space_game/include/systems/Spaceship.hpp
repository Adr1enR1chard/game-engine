#pragma once

#include <DefaultBundle.hpp>
#include <components/Spaceship.hpp>
#include <components/OrbitCamera.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace engine;
using namespace default_bundle;

class SpaceshipControlSystem : public System
{
public:
    void start() override
    {
        m_spaceshipEntity = world().createEntity(
            CModelRenderer{
                .model = services().get<ModelFactory>()->LoadModel("assets/models/spaceship/spaceship.gltf"),
            },
            CTransform{
                .rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            },
            CSpaceship{});
        m_targetRotation =
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void update(float deltaTime) override
    {
        auto [_, cam, cameraTransform, orbitCamera] = world().fetchAt<CCamera, CTransform, COrbitCamera>(0);
        auto [entity, transform] = world().fetchFrom<CTransform>(m_spaceshipEntity);
        if (transform == nullptr)
            return;
        if (cameraTransform == nullptr)
            return;

        float rollDirection = 0.0f;
        if (services().get<Input>()->isKeyDown(Key::A))
        {
            rollDirection += 1.0f;
        }
        if (services().get<Input>()->isKeyDown(Key::D))
        {
            rollDirection -= 1.0f;
        }

        glm::quat delta =
            orbitCamera->yawRot * orbitCamera->pitchRot;
        m_targetRotation = delta * m_targetRotation;

        glm::quat lerpedRotation = glm::slerp(transform->rotation, m_targetRotation, m_rotationSpeed * deltaTime);
        transform->setRotation(lerpedRotation);

        if (services().get<Input>()->isKeyDown(Key::LShift))
        {
            m_velocity += -transform->forward() * m_acceleration * deltaTime;
            if (glm::length(m_velocity) > m_maxSpeed)
                m_velocity = glm::normalize(m_velocity) * m_maxSpeed;
        }
        else if (services().get<Input>()->isKeyDown(Key::LCtrl))
        {
            m_velocity += transform->forward() * m_acceleration * deltaTime;
            if (glm::length(m_velocity) > m_maxSpeed)
                m_velocity = glm::normalize(m_velocity) * m_maxSpeed;
        }
        else
        {
            if (glm::length(m_velocity) > 0.0f)
                m_velocity -= m_deceleration * deltaTime * glm::normalize(m_velocity);
        }

        transform->position += m_velocity * deltaTime;
    }

private:
    float lerpAngle(float current, float target, float alpha)
    {
        float delta = glm::mod(target - current + 180.0f, 360.0f) - 180.0f;
        return current + delta * alpha;
    }

    Entity m_spaceshipEntity;

    glm::vec3 m_velocity = glm::vec3(0.0f);
    float m_maxSpeed = 50.0f;
    float m_acceleration = 20.0f;
    float m_deceleration = 13.0f;
    float m_rotationSpeed = 1.0f;
    glm::quat m_targetRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    const glm::quat yFlip =
        glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0));
};