#include <cassert>
#include <stdexcept>

#include "ecs/system/System.hpp"
#include "ecs/system/SystemScheduler.hpp"

#include <engine/EngineContext.hpp>

class TimeService : public Service
{
public:
    int tick = 0;
};

// A simple system that increments TimeService::tick on every update
class TickSystem : public System
{
public:
    void update(EngineContext &engineContext) override
    {
        engineContext.getService<TimeService>().tick++;
    }
};

int main()
{
    // Arrange: engine context with a TimeService registered
    EngineContext engineContext;
    engineContext.registerService<TimeService>(std::make_unique<TimeService>());

    // Arrange: scheduler with a TickSystem registered
    SystemScheduler scheduler;
    scheduler.registerSystem<TickSystem>();

    // Act: run updates and validate TimeService is used by the system
    scheduler.updateSystems(engineContext);
    assert(engineContext.getService<TimeService>().tick == 1);

    scheduler.updateSystems(engineContext);
    assert(engineContext.getService<TimeService>().tick == 2);

    // Duplicate registration should throw
    bool threwDupReg = !scheduler.registerSystem<TickSystem>();
    assert(threwDupReg);

    // Unregister and ensure further updates don't change the tick
    scheduler.unregisterSystem<TickSystem>();
    scheduler.updateSystems(engineContext);
    assert(engineContext.getService<TimeService>().tick == 2);

    // Unregistering again should throw
    bool threwUnreg = !scheduler.unregisterSystem<TickSystem>();
    assert(threwUnreg);

    return 0;
}
