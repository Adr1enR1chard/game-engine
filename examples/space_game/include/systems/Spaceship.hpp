#pragma once

#include <DefaultBundle.hpp>
#include <components/Spaceship.hpp>

class Spaceship : public System
{
public:
    void start() override
    {
        m_spaceshipEntity = world().createEntity(
            CModelRenderer{
                .modelRef = services().get<ModelFactory>()->LoadModel("assets/models/spaceship/spaceship.gltf"),
            },
            CTransform{glm::vec3(0.0f, 0.0f, 0.0f)}, CSpaceship{});
    }

    void update(float deltaTime) override
    {
        auto [_, cam, cameraTransform, orbitCamera] = world().fetchAt<CCamera, CTransform, COrbitCamera>(0);
        auto [entity, transform] = world().fetchFrom<CTransform>(m_spaceshipEntity);
        if (transform == nullptr)
            return;
        if (cameraTransform == nullptr)
            return;

        m_yaw = lerpAngle(m_yaw, orbitCamera->yaw + 180.0f, m_rotationSpeed * deltaTime);
        m_pitch = lerpAngle(m_pitch, -orbitCamera->pitch, m_rotationSpeed * deltaTime);
        transform->setRotationFromEuler(glm::vec3(glm::radians(m_pitch), glm::radians(m_yaw), 0.0f));

        if (services().get<Input>()->isKeyDown(Key::LShift))
        {
            m_velocity += -transform->forward() * m_acceleration * deltaTime;
            if (glm::length(m_velocity) > m_maxSpeed)
                m_velocity = glm::normalize(m_velocity) * m_maxSpeed;
        }
        else
        {
            if (glm::length(m_velocity) > 0.0f)
                m_velocity -= m_deceleration * deltaTime * glm::normalize(m_velocity);
        }

        // Compute the roll based on lateral movement
        glm::vec3 right = glm::normalize(glm::cross(transform->forward(), glm::vec3(0.0f, 1.0f, 0.0f)));
        float lateralSpeed = glm::dot(m_velocity, right);
        float targetRoll = -lateralSpeed / m_maxSpeed * m_targetRoll;
        m_roll = lerpAngle(m_roll, targetRoll, m_rotationSpeed * deltaTime);
        transform->rotate(glm::radians(m_roll), transform->forward());

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
    float m_maxSpeed = 20.0f;
    float m_acceleration = 10.0f;
    float m_deceleration = 5.0f;
    float m_rotationSpeed = 1.0f;
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
    float m_roll = 0.0f;
    float m_targetRoll = 45.0f;
};