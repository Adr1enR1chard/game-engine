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
        void install(Engine &engine) const override;
        void uninstall(Engine &engine) const override;
    };
}