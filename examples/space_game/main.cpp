#include <DefaultBundle.hpp>

#include <systems/Camera.hpp>
#include <systems/Environment.hpp>
#include <systems/Spaceship.hpp>

int main()
{
    Engine::Init()
        .addBundle<DefaultBundle>()
        .addSystems<Camera, Environment, Spaceship>()
        .run(1920, 1080, "Space Game", true);

    return 0;
}