#include <DefaultBundle.hpp>

#include <systems/Camera.hpp>
#include <systems/Environment.hpp>
#include <systems/Spaceship.hpp>

using namespace engine;
using namespace default_bundle;

int main()
{
    Engine::Init()
        .addBundle<DefaultBundle>()
        .addSystems<OrbitCameraSystem, SpaceEnvironmentSystem, SpaceshipControlSystem>()
        .run(1920, 1080, "Space Game", true);

    return 0;
}