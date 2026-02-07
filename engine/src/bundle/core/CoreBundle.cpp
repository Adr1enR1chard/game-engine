#include "engine/bundle/core/CoreBundle.hpp"

#include "engine/Engine.hpp"

namespace engine
{
    void CoreBundle::install(Engine &engine) const
    {
        engine.addService<Renderer>();
        engine.addService<TextureFactory>(*(engine.getService<Renderer>()));
        engine.addService<MeshFactory>(*(engine.getService<Renderer>()));
        engine.addService<ShaderFactory>(*(engine.getService<Renderer>()), *(engine.getService<TextureFactory>()));

        engine.addService<MaterialFactory>(*(engine.getService<Renderer>()), *(engine.getService<ShaderFactory>()));
        engine.addService<ModelFactory>(*(engine.getService<MaterialFactory>()), *(engine.getService<ShaderFactory>()),
                                        *(engine.getService<TextureFactory>()), *(engine.getService<MeshFactory>()));

        engine.addSystems<TransformSystem, CameraSystem, LightSystem>();
    }
    void CoreBundle::uninstall(Engine &engine) const
    {
        engine.removeServices<Renderer, TextureFactory, MeshFactory, ShaderFactory, MaterialFactory, ModelFactory>();
        engine.removeSystems<TransformSystem, CameraSystem, LightSystem>();
    }

} // namespace engine