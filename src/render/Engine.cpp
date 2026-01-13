#include <engine/Engine.hpp>

#include <engine/Input.hpp>
#include <engine/Window.hpp>

Engine::Engine(int width, int height, const char* title, bool fullscreen)
{
    m_world.Serv<Input>();
    m_world.Serv<Window>().init(width, height, title, fullscreen);
}

Engine::~Engine()
{
    for (auto& bundle : m_bundles) {
        delete bundle;
    }
};

void Engine::run()
{
    Window& window = m_world.Serv<Window>();
    Input&  input  = m_world.Serv<Input>();

    using clock  = std::chrono::steady_clock;
    using frames = std::chrono::duration<int, std::ratio<1, 144>>;

    auto lastFrame = clock::now();

    m_systemScheduler.startSystems(m_world);

    while (!window.shouldClose()) {
        auto                          frameStart = clock::now();
        std::chrono::duration<double> delta      = frameStart - lastFrame;
        lastFrame                                = frameStart;

        // --- Engine loop ---
        window.clear();

        m_systemScheduler.updateSystems(m_world, delta.count());

        input.clear();

        window.swapBuffers();
        window.pollEvents();
    }
}