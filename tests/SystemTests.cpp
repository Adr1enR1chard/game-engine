#include <cassert>
#include <stdexcept>

#include "scene/Scene.hpp"
#include "ecs/system/System.hpp"

// Test systems
struct TestSystemA : System
{
    static int updates;
    static float lastDt;
    static Scene *lastScene;

    void update(Scene &scene, float deltaTime) override
    {
        updates++;
        lastDt = deltaTime;
        lastScene = &scene;
    }
};

int TestSystemA::updates = 0;
float TestSystemA::lastDt = 0.0f;
Scene *TestSystemA::lastScene = nullptr;

struct TestSystemB : System
{
    static int updates;
    static float lastDt;
    static Scene *lastScene;

    void update(Scene &scene, float deltaTime) override
    {
        updates++;
        lastDt = deltaTime;
        lastScene = &scene;
    }
};

int TestSystemB::updates = 0;
float TestSystemB::lastDt = 0.0f;
Scene *TestSystemB::lastScene = nullptr;

int main()
{
    // Create a scene and get its scheduler
    Scene scene;
    auto &scheduler = scene.getSystemScheduler();

    // Unregistering a non-registered system should throw
    bool threwUnregisterBeforeRegister = false;
    try
    {
        scheduler.unregisterSystem<TestSystemA>();
    }
    catch (const std::runtime_error &)
    {
        threwUnregisterBeforeRegister = true;
    }
    assert(threwUnregisterBeforeRegister);

    // Register a system successfully
    scheduler.registerSystem<TestSystemA>();

    // Double registration should throw
    bool threwDoubleRegister = false;
    try
    {
        scheduler.registerSystem<TestSystemA>();
    }
    catch (const std::runtime_error &)
    {
        threwDoubleRegister = true;
    }
    assert(threwDoubleRegister);

    // Register a second distinct system
    scheduler.registerSystem<TestSystemB>();

    // Reset counters
    TestSystemA::updates = 0;
    TestSystemA::lastDt = 0.0f;
    TestSystemA::lastScene = nullptr;
    TestSystemB::updates = 0;
    TestSystemB::lastDt = 0.0f;
    TestSystemB::lastScene = nullptr;

    // Update should call both systems once
    const float dt1 = 0.25f;
    scheduler.updateSystems(dt1);
    assert(TestSystemA::updates == 1);
    assert(TestSystemB::updates == 1);
    assert(TestSystemA::lastDt == dt1);
    assert(TestSystemB::lastDt == dt1);
    assert(TestSystemA::lastScene == &scene);
    assert(TestSystemB::lastScene == &scene);

    // Unregister first system, only the second should update
    scheduler.unregisterSystem<TestSystemA>();
    const float dt2 = 0.5f;
    scheduler.updateSystems(dt2);
    assert(TestSystemA::updates == 1); // unchanged
    assert(TestSystemB::updates == 2); // incremented
    assert(TestSystemB::lastDt == dt2);
    assert(TestSystemB::lastScene == &scene);

    // Unregistering already unregistered should throw
    bool threwUnregisterAgain = false;
    try
    {
        scheduler.unregisterSystem<TestSystemA>();
    }
    catch (const std::runtime_error &)
    {
        threwUnregisterAgain = true;
    }
    assert(threwUnregisterAgain);

    return 0;
}
