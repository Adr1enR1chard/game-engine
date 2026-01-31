#include <engine/Engine.hpp>

std::unique_ptr<Engine> Engine::m_instance = nullptr;

Engine::Engine() : m_running(false)
{
    m_services.add<Window>();
    m_services.add<Input>();
    m_services.add<TextureResource>();
    m_services.add<ShaderResource>();
    m_services.add<MeshResource>();
    m_services.add<MaterialResource>();
    m_services.add<ModelResource>();

    m_window = m_services.get<Window>();
    m_input = m_services.get<Input>();
}
