#include <engine/Engine.hpp>
#include <engine/bundle/core/CoreBundle.hpp>
#include <engine/bundle/standalone/StandaloneBundle.hpp>

namespace engine
{

    std::unique_ptr<Engine> Engine::m_instance = nullptr;

    Engine::Engine() : m_running(false) {}

    Engine &Engine::InitializeStandalone()
    {
        if (!m_instance)
        {
            m_instance.reset(new Engine());
        }

        m_instance->addBundle<CoreBundle>().addBundle<StandaloneBundle>();

        return *m_instance;
    }

    Engine &Engine::InitializeEmbedded(World *&outWorld, SystemRegistry *&outSystems, ServiceRegistry *&outServices)
    {
        if (!m_instance)
        {
            m_instance.reset(new Engine());
        }
        m_instance->addBundle<CoreBundle>();
        outWorld = &m_instance->m_world;
        outSystems = &m_instance->m_systems;
        outServices = &m_instance->m_services;

        return *m_instance;
    }

    void Engine::Shutdown()
    {
        if (!m_instance)
            return;

        Log::Print("Engine Shutdown", LogLevel::Info);
        m_instance->m_running = false;
    }

    void Engine::ExecuteOnMainThread(const std::function<void()> &task)
    {
        if (m_instance)
        {
            m_instance->m_mainThreadQueue.enqueue(task);
        }
    }

    void Engine::run(unsigned int width, unsigned int height, const char *title, bool fullscreen)
    {
        auto *window = m_services.get<Window>();
        auto *input = m_services.get<Input>();

        if (!window || !input)
        {
            Log::Print("Engine must be initialized with StandaloneBundle before running. Use Engine::InitializeStandalone().", LogLevel::Critical);
            throw std::runtime_error("Engine must be initialized with StandaloneBundle before running.");
            return;
        }

        window->setResolution(width, height);
        window->setTitle(title);
        window->setFullscreen(fullscreen);
        window->create();

        m_running = true;

        using clock = std::chrono::steady_clock;
        auto lastFrame = clock::now();

        float deltaTime = 0.0f;

        m_systems.start();
        while (m_running)
        {
            auto frameStart = clock::now();
            std::chrono::duration<double> delta = frameStart - lastFrame;
            lastFrame = frameStart;

            deltaTime = static_cast<float>(delta.count());

            // --- Engine loop ---
            input->clear();
            window->pollEvents();
            auto *renderer = m_services.get<Renderer>();
            renderer->clear(true, true, false);
            m_systems.update(deltaTime);
            window->swapBuffers();

            m_mainThreadQueue.executeAll();
        }
    }

} // namespace engine
