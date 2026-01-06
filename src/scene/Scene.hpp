#pragma once

#include "ecs/registry/Registry.hpp"
#include "ecs/system/SystemScheduler.hpp"

#include "ecs/component/CTransform.hpp"
#include "ecs/component/CTransformCache.hpp"
#include "ecs/component/CMeshRenderer.hpp"
#include "ecs/component/CCamera.hpp"
#include "ecs/component/CCameraCache.hpp"
#include "ecs/system/TransformSystem.hpp"
#include "ecs/system/RenderSystem.hpp"
#include <ecs/system/CameraSystem.hpp>

class Scene
{
private:
    Registry m_registry;
    SystemScheduler m_systemScheduler;

public:
    Scene()
    {
        m_systemScheduler.registerSystem<TransformSystem>();
        m_systemScheduler.registerSystem<CameraSystem>();
        m_systemScheduler.registerSystem<RenderSystem>();
    }

    ~Scene()
    {
    }

    Registry &registry() { return m_registry; }

    SystemScheduler &systems() { return m_systemScheduler; }
};