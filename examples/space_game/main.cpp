#include <RasterizationBundle.hpp>
#include <engine/Engine.hpp>

#include <systems/Camera.hpp>
#include <systems/Environment.hpp>
#include <systems/Spaceship.hpp>

using namespace engine;
using namespace rasterization_bundle;

int main()
{
    Engine::InitializeStandalone()
        .addBundle<RasterizationBundle>()
        .addSystems<OrbitCameraSystem, SpaceEnvironmentSystem, SpaceshipControlSystem>()
        .run(1920, 1080, "Space Game", true);

    return 0;
}