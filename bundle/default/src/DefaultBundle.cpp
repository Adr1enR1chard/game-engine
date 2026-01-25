#include <DefaultBundle.hpp>

void DefaultBundle::install(SystemRegistry& systems, ServiceRegistry& services) const
{
    systems.add<TransformSystem, CameraSystem, RenderSystem, LightSystem, PlatformSystem, EnvironmentSystem>();
    services.add<Window, Input>();
    services.add<MeshResource, MaterialResource, TextureResource, ShaderResource, ModelResource>();
    services.add<TextureFactory>(*(services.get<TextureResource>()));
    services.add<MeshFactory>(*(services.get<MeshResource>()));
    services.add<ShaderFactory>(*(services.get<ShaderResource>()), *(services.get<TextureFactory>()));
    services.add<MaterialFactory>(*(services.get<MaterialResource>()), *(services.get<ShaderFactory>()));
    services.add<ModelFactory>(*(services.get<MaterialFactory>()), *(services.get<ShaderFactory>()),
                               *(services.get<TextureFactory>()), *(services.get<MeshFactory>()),
                               *(services.get<ModelResource>()));
}
void DefaultBundle::uninstall(SystemRegistry& systems, ServiceRegistry& services) const
{
    systems.remove<TransformSystem, CameraSystem, RenderSystem, LightSystem, PlatformSystem, EnvironmentSystem>();
    services.remove<Window, Input, MeshResource, MaterialResource, TextureResource, ShaderResource, ModelResource>();
    services.remove<MaterialFactory>();
    services.remove<TextureFactory>();
    services.remove<MeshFactory>();
    services.remove<ModelResource>();
    services.remove<ShaderFactory>();
    services.remove<ModelFactory>();
}