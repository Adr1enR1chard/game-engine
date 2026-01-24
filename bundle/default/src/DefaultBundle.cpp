#include <DefaultBundle.hpp>

void DefaultBundle::install(SystemRegistry& systems, ServiceRegistry& services) const
{
    systems.add<TransformSystem, CameraSystem, RenderSystem, LightSystem, PlatformSystem, EnvironmentSystem>();
    services.add<Window, Input, MeshResource, MaterialResource, TextureResource>();
    services.add<ShaderResource>(*(services.get<TextureResource>()));
    services.add<MaterialFactory>(*(services.get<MaterialResource>()), *(services.get<ShaderResource>()));
    services.add<TextureFactory>(*(services.get<TextureResource>()));
    services.add<MeshFactory>(*(services.get<MeshResource>()));
    services.add<ModelResource>(*(services.get<MeshResource>()));
}
void DefaultBundle::uninstall(SystemRegistry& systems, ServiceRegistry& services) const
{
    systems.remove<TransformSystem, CameraSystem, RenderSystem, LightSystem, PlatformSystem>();
    services.remove<Window, Input, MeshResource, MaterialResource, TextureResource>();
    services.remove<ShaderResource>();
    services.remove<MaterialFactory>();
    services.remove<TextureFactory>();
}