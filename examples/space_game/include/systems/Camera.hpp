#pragma once

#include <DefaultBundle.hpp>
#include <components/Spaceship.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <components/OrbitCamera.hpp>

class Camera : public System
{
public:
    void start() override
    {
        m_cameraEntity = world().createEntity(CCamera{}, CTransform{
                                                             .rotation = glm::vec3(0.0f, 0.0f, 0.0f),
                                                         },
                                              COrbitCamera{});

        m_input = services().get<Input>();
        m_window = services().get<Window>();
    }

    void update(float deltaTime) override
    {
        float dt = static_cast<float>(deltaTime);
        if (auto [entity, cCam, transform, orbitCamera] = world().fetchAt<CCamera, CTransform, COrbitCamera>(0); entity != 0 && m_mouseCaptured)
        {
            glm::vec2 mouseDelta = m_input->getMouseDelta();
            float sensitivity = 0.2f;

            orbitCamera->yawDelta = -mouseDelta.x * sensitivity;
            orbitCamera->yaw += -mouseDelta.x * sensitivity;
            orbitCamera->pitch += -mouseDelta.y * sensitivity;

            orbitCamera->pitch = glm::clamp(orbitCamera->pitch, -89.0f, 89.0f);

            transform->setRotationFromEuler(glm::vec3(
                glm::radians(orbitCamera->pitch),
                glm::radians(orbitCamera->yaw),
                0.0f));

            // Follow the spaceship
            if (auto [e, s, shipTransform] = world().fetchAt<CSpaceship, CTransform>(0);
                shipTransform != nullptr)
            {
                transform->position =
                    shipTransform->position - transform->forward() * m_orbitRadius +
                    glm::vec3(0.0f, m_orbitHeight, 0.0f);
            }
        }

        if (m_input->isKeyPressed(Key::Escape))
        {
            m_window->captureMouse(false);
            m_mouseCaptured = false;
        }
        if (m_input->isKeyPressed(Key::LAlt))
        {
            m_window->captureMouse(true);
            m_mouseCaptured = true;
        }
    }

private:
    Entity m_cameraEntity;
    Input *m_input;
    Window *m_window;
    bool m_mouseCaptured = true;
    float m_orbitRadius = 20.0f;
    float m_orbitHeight = 5.0f;
};