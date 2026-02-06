#include <DefaultBundle.hpp>

#include <engine/Engine.hpp>

namespace default_bundle
{
    using namespace engine;

    void DefaultBundle::install(Engine &engine) const
    {
        if (!engine.getService<Renderer>())
        {
            Log::Print("DefaultBundle requires Renderer service to be installed.", LogLevel::Critical);
            throw std::runtime_error("DefaultBundle requires Renderer service to be installed.");
            return;
        }

        engine.addService<TextureFactory>(*(engine.getService<Renderer>()));
        engine.addService<MeshFactory>(*(engine.getService<Renderer>()));
        engine.addService<ShaderFactory>(*(engine.getService<Renderer>()), *(engine.getService<TextureFactory>()));

        engine.addService<MaterialFactory>(*(engine.getService<Renderer>()), *(engine.getService<ShaderFactory>()));
        engine.addService<ModelFactory>(*(engine.getService<MaterialFactory>()), *(engine.getService<ShaderFactory>()),
                                        *(engine.getService<TextureFactory>()), *(engine.getService<MeshFactory>()));

        engine.addService<ShadowMapping>(*(engine.getService<Renderer>()), *(engine.getService<ShaderFactory>()));
        engine.addSystems<TransformSystem, CameraSystem, RenderSystem, LightSystem, EnvironmentSystem>();
    }
    void DefaultBundle::uninstall(Engine &engine) const
    {
        engine.removeSystems<TransformSystem, CameraSystem, RenderSystem, LightSystem, EnvironmentSystem>();
        engine.removeServices<MaterialFactory, TextureFactory, MeshFactory, ModelFactory, ShaderFactory, ShadowMapping>();
    }

} // namespace default_bundle