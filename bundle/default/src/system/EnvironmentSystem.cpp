#include "system/EnvironmentSystem.hpp"

#include <component/CEnvironment.hpp>
#include <component/cache/CSkyboxCache.hpp>
#include <engine/utils/Log.hpp>
#include <model/MaterialInstance.hpp>
#include <model/Mesh.hpp>

void EnvironmentSystem::start()
{
    if (auto [envEntity, environment] = world().getAt<CEnvironment>(0); envEntity) {
        if (environment->skybox.isValid()) {
            world().add(envEntity, CSkyboxCache{
                                       &environment->skybox,
                                       Mesh::Cube(),
                                       MaterialInstance::Skybox({.colorMap = environment->skybox}),
                                   });
            Log::Print("Skybox loaded into environment system.", LogLevel::Info);
        }
    }
}

void EnvironmentSystem::render(double /*deltaTime*/) {}