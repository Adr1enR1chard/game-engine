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

            glm::vec3 right = transform->right();
            glm::vec3 up = transform->up();

            orbitCamera->yawRot = glm::angleAxis(glm::radians(-mouseDelta.x * sensitivity), up);
            orbitCamera->pitchRot = glm::angleAxis(glm::radians(-mouseDelta.y * sensitivity), right);

            transform->rotate(orbitCamera->yawRot * orbitCamera->pitchRot);

            // Follow the spaceship
            if (auto [e, s, shipTransform] = world().fetchAt<CSpaceship, CTransform>(0);
                shipTransform != nullptr)
            {
                transform->position =
                    shipTransform->position - transform->forward() * m_orbitRadius +
                    transform->up() * m_orbitHeight;
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