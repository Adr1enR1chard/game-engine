#pragma once

#include "ecs/component/CComponent.hpp"

#include <glm/glm.hpp>

struct CTransform : CComponent
{
public:
    const glm::vec3 &getPosition() { return this->position; }
    void setPosition(const glm::vec3 &pos)
    {
        this->position = pos;
        this->dirty = true;
    }

    const glm::vec3 &getRotation() { return this->rotation; }
    void setRotation(const glm::vec3 &rot)
    {
        this->rotation = rot;
        this->dirty = true;
    }

    const glm::vec3 &getScale() { return this->scale; }
    void setScale(const glm::vec3 &scl)
    {
        this->scale = scl;
        this->dirty = true;
    }

private:
    bool isDirty() const { return this->dirty; }
    void setDirty(bool value) { this->dirty = value; }

    friend class TransformSystem;

private:
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec3 rotation{0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};

    bool dirty = true;
};