#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include "component.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform : public Component
{
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

public:
    Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
        : position(position), rotation(rotation), scale(scale) {}

    glm::mat4 getModelMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }

    glm::mat4 getViewMatrix() const
    {
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::rotate(view, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, position);
        return view;
    }
};

#endif