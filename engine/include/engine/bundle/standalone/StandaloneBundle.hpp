#pragma once

#include <engine/model/Bundle.hpp>

#include <engine/bundle/standalone/service/Input.hpp>
#include <engine/bundle/standalone/service/Renderer.hpp>
#include <engine/bundle/standalone/service/Window.hpp>

namespace engine
{
    class StandaloneBundle : public Bundle
    {
    public:
        void install(SystemRegistry &systems, ServiceRegistry &services) const override;
        void uninstall(SystemRegistry &systems, ServiceRegistry &services) const override;
    };
}