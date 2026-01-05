#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
    glm::vec3 position;
    glm::vec3 rotation;

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    bool isActive;

public:
    Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f), float fov = 45.0f, float aspectRatio = 4.0f / 3.0f, float nearPlane = 0.1f, float farPlane = 100.0f) : position(position), rotation(rotation), fov(fov), aspectRatio(aspectRatio), nearPlane(nearPlane), farPlane(farPlane), isActive(true) {}
    ~Camera() {};

    bool getIsActive() const { return isActive; }
    void setIsActive(bool active) { isActive = active; }

    glm::vec3 getPosition() const { return position; }
    void setPosition(const glm::vec3 &pos) { position = pos; }

    glm::vec3 getRotation() const { return rotation; }
    void setRotation(const glm::vec3 &rot) { rotation = rot; }

    glm::mat4 getViewMatrix() const
    {
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::rotate(view, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, position);
        return view;
    }

    glm::mat4 getProjectionMatrix() const
    {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }
};

#endif