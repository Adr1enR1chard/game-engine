#include <RasterizationBundle.hpp>

#include <engine/Engine.hpp>
#include <engine/bundle/core/service/factory/ShaderFactory.hpp>

namespace rasterization_bundle
{
    using namespace engine;

    void RasterizationBundle::install(Engine &engine) const
    {
        engine.addService<ShadowMapping>(*(engine.getService<Renderer>()), *(engine.getService<ShaderFactory>()));
        engine.addSystems<RenderSystem, EnvironmentSystem>();
    }
    void RasterizationBundle::uninstall(Engine &engine) const
    {
        engine.removeSystems<RenderSystem, EnvironmentSystem>();
        engine.removeServices<ShadowMapping>();
    }

} // namespace rasterization_bundle