#include <DefaultBundle.hpp>

namespace default_bundle
{

    using namespace engine;

    void DefaultBundle::install(SystemRegistry &systems, ServiceRegistry &services) const
    {
        systems.add<TransformSystem, CameraSystem, RenderSystem, LightSystem, EnvironmentSystem>();
        services.add<TextureFactory>(*(services.get<TextureResource>()));
        services.add<MeshFactory>(*(services.get<MeshResource>()));
        services.add<ShaderFactory>(*(services.get<ShaderResource>()), *(services.get<TextureFactory>()));
        services.add<MaterialFactory>(*(services.get<MaterialResource>()), *(services.get<ShaderFactory>()));
        services.add<ModelFactory>(*(services.get<MaterialFactory>()), *(services.get<ShaderFactory>()),
                                   *(services.get<TextureFactory>()), *(services.get<MeshFactory>()),
                                   *(services.get<ModelResource>()));
        services.add<ShadowMapping>(*(services.get<TextureResource>()), *(services.get<ShaderFactory>()));
    }
    void DefaultBundle::uninstall(SystemRegistry &systems, ServiceRegistry &services) const
    {
        systems.remove<TransformSystem, CameraSystem, RenderSystem, LightSystem, EnvironmentSystem>();
        services.remove<MaterialFactory>();
        services.remove<TextureFactory>();
        services.remove<MeshFactory>();
        services.remove<ModelResource>();
        services.remove<ShaderFactory>();
        services.remove<ModelFactory>();
    }

} // namespace default_bundle