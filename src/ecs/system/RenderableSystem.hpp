#pragma once

#include <glm/glm.hpp>

#include "ecs/component/CTransform.hpp"
#include "ecs/component/CMeshRenderer.hpp"

#include "ecs/system/System.hpp"

class RenderableSystem : public System
{
public:
    void update(Scene &scene, float deltaTime) override;
};