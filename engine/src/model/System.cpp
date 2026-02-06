#pragma once

#include <engine/model/System.hpp>
#include <engine/registry/ServiceRegistry.hpp>
#include <engine/registry/World.hpp>

namespace engine
{
    World &System::world()
    {
        return *m_world;
    }

    ServiceRegistry &System::services()
    {
        return *m_services;
    }

}