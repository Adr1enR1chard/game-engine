#include <DefaultBundle.hpp>

namespace default_bundle
{

    using namespace engine;

    void DefaultBundle::install(SystemRegistry &systems, ServiceRegistry &services) const
    {
        systems.add<TransformSystem, CameraSystem, RenderSystem, LightSystem, EnvironmentSystem>();

        if (!services.get<Renderer>() || !services.get<Window>())
        {
            Log::Print("DefaultBundle requires Renderer and Window services to be installed.", LogLevel::Critical);
            throw std::runtime_error("DefaultBundle requires Renderer and Window services to be installed.");
            return;
        }

        services.add<TextureFactory>(*(services.get<Renderer>()));
        services.add<MeshFactory>(*(services.get<Renderer>()));
        services.add<ShaderFactory>(*(services.get<Renderer>()), *(services.get<TextureFactory>()));

        services.add<MaterialFactory>(*(services.get<Renderer>()), *(services.get<ShaderFactory>()));
        services.add<ModelFactory>(*(services.get<MaterialFactory>()), *(services.get<ShaderFactory>()),
                                   *(services.get<TextureFactory>()), *(services.get<MeshFactory>()));

        services.add<ShadowMapping>(*(services.get<Renderer>()), *(services.get<ShaderFactory>()));
    }
    void DefaultBundle::uninstall(SystemRegistry &systems, ServiceRegistry &services) const
    {
        systems.remove<TransformSystem, CameraSystem, RenderSystem, LightSystem, EnvironmentSystem>();
        services.remove<MaterialFactory>();
        services.remove<TextureFactory>();
        services.remove<MeshFactory>();
        services.remove<ModelFactory>();
        services.remove<ShaderFactory>();
    }

} // namespace default_bundle