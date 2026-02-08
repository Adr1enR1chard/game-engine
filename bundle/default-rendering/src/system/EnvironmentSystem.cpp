#include "system/EnvironmentSystem.hpp"

#include <engine/bundle/core/component/CEnvironment.hpp>
#include <engine/utils/Log.hpp>
#include <engine/bundle/core/service/factory/MeshFactory.hpp>
#include <engine/bundle/standalone/service/Window.hpp>

#include <component/CSkyboxCache.hpp>

namespace default_rendering
{

    using namespace engine;

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
            // TODO: replace with renderer
            // services().get<Window>()->clearColor(m_environment->backgroundColor);
            m_lastClearColor = m_environment->backgroundColor;
        }
    }

    CSkyboxCache EnvironmentSystem::createSkyboxCache()
    {
        return CSkyboxCache{
            .meshRef = services().get<MeshFactory>()->Cube(),
        };
    }

} // namespace engine
