#pragma once
#include <DefaultBundle.hpp>

class CameraControlSystem : public System
{
  public:
    void update(double deltaTime) override
    {
        float dt = static_cast<float>(deltaTime);
        if (auto [entity, cCam, transform] = world().getAt<CCamera, CTransform>(0); entity != 0) {
            Input* input = services().get<Input>();
            transform->rotation.y += -input->getMouseDelta().x * 0.2f;
            transform->rotation.x += -input->getMouseDelta().y * 0.2f;

            transform->rotation.x = glm::clamp(transform->rotation.x, -89.0f, 89.0f);

            float yaw   = glm::radians(transform->rotation.y);
            float pitch = glm::radians(transform->rotation.x);

            glm::vec3 forward;
            forward.x = -cos(pitch) * sin(yaw);
            forward.y = sin(pitch);
            forward.z = -cos(pitch) * cos(yaw);

            forward = glm::normalize(forward);

            glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

            glm::vec2 direction(0.0f);

            if (input->isKeyDown(Key::A))
                direction.x = -1;
            if (input->isKeyDown(Key::D))
                direction.x = 1;
            if (input->isKeyDown(Key::W))
                direction.y = 1;
            if (input->isKeyDown(Key::S))
                direction.y = -1;
            direction = glm::length(direction) != 0 ? glm::normalize(direction) : direction;

            float speed = 5.0f * dt;
            if (input->isKeyDown(Key::LShift))
                speed *= 2.0f;

            transform->position += forward * direction.y * speed;
            transform->position += right * direction.x * speed;
        }
    }
};