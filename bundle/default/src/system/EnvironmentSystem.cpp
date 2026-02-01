#include "system/EnvironmentSystem.hpp"

#include <component/CEnvironment.hpp>
#include <component/cache/CSkyboxCache.hpp>
#include <engine/utils/Log.hpp>
#include <service/factory/MeshFactory.hpp>
#include <engine/service/platform/Window.hpp>

void EnvironmentSystem::update(float /*deltaTime*/)
{
    // Fetch the environment entity if not already done
    if (auto [envEntity, environment] = world().fetchAt<CEnvironment>(0); envEntity && !m_environment)
    {
        m_environment = environment;
        world().addComponents(envEntity, createSkyboxCache());
    }

    if (!m_environment)
        return;

    if (m_lastClearColor != m_environment->backgroundColor)
    {
        services().get<Window>()->clearColor(m_environment->backgroundColor);
        m_lastClearColor = m_environment->backgroundColor;
    }
}

CSkyboxCache EnvironmentSystem::createSkyboxCache()
{
    return CSkyboxCache{
        .meshRef = services().get<MeshFactory>()->Cube(),
    };
}
