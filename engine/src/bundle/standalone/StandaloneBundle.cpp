#include <engine/bundle/standalone/StandaloneBundle.hpp>

#include <engine/registry/SystemRegistry.hpp>
#include <engine/registry/ServiceRegistry.hpp>

namespace engine
{
    void StandaloneBundle::install(SystemRegistry & /**/, ServiceRegistry &services) const
    {
        services.add<Window>();
        services.add<Input>();
        services.add<Renderer>();
    }

    void StandaloneBundle::uninstall(SystemRegistry & /**/, ServiceRegistry &services) const
    {
        services.remove<Renderer>();
        services.remove<Input>();
        services.remove<Window>();
    }
}