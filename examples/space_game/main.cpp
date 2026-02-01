#include <DefaultBundle.hpp>

#include <systems/Camera.hpp>
#include <systems/Environment.hpp>
#include <systems/Spaceship.hpp>

int main()
{
    Engine::Init()
        .addBundle<DefaultBundle>()
        .addSystems<Camera, Environment, Spaceship>()
        .run(1280, 720, "Space Game", false);

    return 0;
}