#pragma once
#include <engine/bundle/core/CoreBundle.hpp>

using namespace engine;

class CameraControlSystem : public System
{
public:
    void start() override
    {
        input = services().get<Input>();
        window = services().get<Window>();
    }

    void update(float deltaTime) override
    {
        float dt = static_cast<float>(deltaTime);
        if (auto [entity, cCam, transform] = world().fetchAt<CCamera, CTransform>(0); entity != 0 && mouseCaptured)
        {
            transform->rotate(glm::radians(-input->getMouseDelta().y * 0.2f), transform->right());
            transform->rotate(glm::radians(-input->getMouseDelta().x * 0.2f), glm::vec3(0, 1, 0));

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

            transform->position += transform->forward() * direction.y * speed;
            transform->position += transform->right() * direction.x * speed;
        }

        if (input->isKeyPressed(Key::Escape))
        {
            window->captureMouse(false);
            mouseCaptured = false;
        }
        if (input->isKeyPressed(Key::LAlt))
        {
            window->captureMouse(true);
            mouseCaptured = true;
        }
    }

private:
    Input *input = nullptr;
    Window *window = nullptr;
    bool mouseCaptured = true;
};