#include <engine/bundle/standalone/StandaloneBundle.hpp>

#include <engine/Engine.hpp>

namespace engine
{
    void StandaloneBundle::install(Engine &engine) const
    {
        engine.addService<Window>();
        engine.addService<Input>();
    }

    void StandaloneBundle::uninstall(Engine &engine) const
    {
        engine.removeServices<Window, Input>();
    }
}