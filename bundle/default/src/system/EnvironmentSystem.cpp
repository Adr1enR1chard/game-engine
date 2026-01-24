#include "system/EnvironmentSystem.hpp"

#include <component/CEnvironment.hpp>
#include <component/cache/CSkyboxCache.hpp>
#include <engine/utils/Log.hpp>
#include <model/MaterialInstance.hpp>
#include <model/Mesh.hpp>
#include <service/factory/MeshFactory.hpp>

void EnvironmentSystem::start()
{
    if (auto [envEntity, environment] = world().getAt<CEnvironment>(0); envEntity) {
        if (environment->skyboxMaterial != 0) {
            world().add(envEntity, CSkyboxCache{
                                       services().get<MeshFactory>()->Cube(),
                                   });
        }
    }
}

void EnvironmentSystem::render(double /*deltaTime*/) {}