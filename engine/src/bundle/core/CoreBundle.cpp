#include "engine/bundle/core/CoreBundle.hpp"

#include "engine/Engine.hpp"

namespace engine
{
    void CoreBundle::install(Engine &engine) const
    {
        engine.addService<Renderer>();
    }
    void CoreBundle::uninstall(Engine &engine) const
    {
        engine.removeServices<Renderer>();
    }

} // namespace engine