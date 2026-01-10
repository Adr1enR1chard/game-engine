#include <cassert>
#include <stdexcept>

#include "ecs/Registry.hpp"

struct Position {
    int x = 0;
    int y = 0;
};

struct Velocity {
    float vx = 0.f;
    float vy = 0.f;
};

struct Health {
    int hp = 100;
};

int main()
{
    Registry reg;

    // Create first entity
    Entity e1 = reg.createEntity();

    // Create and get component successfully
    Position& p1 = reg.createComponent<Position>(e1);
    p1.x         = 10;
    p1.y         = 20;

    Position& p1Get = reg.getComponent<Position>(e1);
    assert(&p1Get == &p1);
    assert(p1Get.x == 10);
    assert(p1Get.y == 20);

    // Create Velocity for e1 and validate retrieval
    Velocity& v1    = reg.createComponent<Velocity>(e1);
    v1.vx           = 1.5f;
    v1.vy           = -0.5f;
    Velocity& v1Get = reg.getComponent<Velocity>(e1);
    assert(&v1Get == &v1);
    assert(v1Get.vx == 1.5f);
    assert(v1Get.vy == -0.5f);

    return 0;
}