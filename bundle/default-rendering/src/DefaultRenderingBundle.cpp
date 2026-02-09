#include <DefaultRenderingBundle.hpp>

#include <engine/Engine.hpp>
#include <engine/bundle/core/service/factory/ShaderFactory.hpp>

namespace default_rendering
{
    using namespace engine;

    void DefaultRendering::install(Engine &engine) const
    {
        engine.addService<ShadowMapping>(*(engine.getService<Renderer>()), *(engine.getService<ShaderFactory>()));
        engine.addService<SSR>();
        engine.addSystems<RenderSystem, EnvironmentSystem>();
    }
    void DefaultRendering::uninstall(Engine &engine) const
    {
        engine.removeSystems<RenderSystem, EnvironmentSystem>();
        engine.removeServices<ShadowMapping, SSR>();
    }

} // namespace default_rendering