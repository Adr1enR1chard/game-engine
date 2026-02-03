#include <engine/Engine.hpp>

namespace engine
{

    std::unique_ptr<Engine> Engine::m_instance = nullptr;

    Engine::Engine() : m_running(false)
    {
        m_services.add<Window>();
        m_services.add<Input>();
        m_services.add<Renderer>();

        m_window = m_services.get<Window>();
        m_input = m_services.get<Input>();
    }

} // namespace engine
