#include <cassert>
#include <stdexcept>

#include "ecs/registry/Registry.hpp"

struct Position : CComponent
{
    int x = 0;
    int y = 0;
    inline const bool isValid() const override
    {
        return true;
    }
};

struct Velocity : CComponent
{
    float vx = 0.f;
    float vy = 0.f;
    inline const bool isValid() const override
    {
        return true;
    }
};

struct Health : CComponent
{
    int hp = 100;
    inline const bool isValid() const override
    {
        return true;
    }
};

int main()
{
    Registry reg;

    // Create first entity
    Entity e1 = reg.createEntity();

    // Creating component before registration should throw
    bool threwCreateBeforeReg = false;
    try
    {
        (void)reg.createComponent<Position>(e1);
    }
    catch (const std::runtime_error &)
    {
        threwCreateBeforeReg = true;
    }
    assert(threwCreateBeforeReg);

    // Getting component before registration should throw
    bool threwGetBeforeReg = false;
    try
    {
        (void)reg.getComponent<Position>(e1);
    }
    catch (const std::runtime_error &)
    {
        threwGetBeforeReg = true;
    }
    assert(threwGetBeforeReg);

    // Register components
    reg.registerComponent<Position>();
    reg.registerComponent<Velocity>();

    // Duplicate registration should throw
    bool threwDupReg = false;
    try
    {
        reg.registerComponent<Position>();
    }
    catch (const std::runtime_error &)
    {
        threwDupReg = true;
    }
    assert(threwDupReg);

    // Create and get component successfully
    Position &p1 = reg.createComponent<Position>(e1);
    p1.x = 10;
    p1.y = 20;

    Position &p1Get = reg.getComponent<Position>(e1);
    assert(&p1Get == &p1);
    assert(p1Get.x == 10);
    assert(p1Get.y == 20);

    // Creating component of same type for same entity should throw
    bool threwDupCreate = false;
    try
    {
        (void)reg.createComponent<Position>(e1);
    }
    catch (const std::runtime_error &)
    {
        threwDupCreate = true;
    }
    assert(threwDupCreate);

    // Second entity without Position
    Entity e2 = reg.createEntity();
    bool threwGetMissing = false;
    try
    {
        (void)reg.getComponent<Position>(e2);
    }
    catch (const std::runtime_error &)
    {
        threwGetMissing = true;
    }
    assert(threwGetMissing);

    // Try getting not-registered component type
    bool threwGetUnregisteredType = false;
    try
    {
        (void)reg.getComponent<Health>(e1);
    }
    catch (const std::runtime_error &)
    {
        threwGetUnregisteredType = true;
    }
    assert(threwGetUnregisteredType);

    // Create Velocity for e1 and validate retrieval
    Velocity &v1 = reg.createComponent<Velocity>(e1);
    v1.vx = 1.5f;
    v1.vy = -0.5f;
    Velocity &v1Get = reg.getComponent<Velocity>(e1);
    assert(&v1Get == &v1);
    assert(v1Get.vx == 1.5f);
    assert(v1Get.vy == -0.5f);

    return 0;
}