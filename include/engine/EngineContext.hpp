#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <stdexcept>

#include "utils/types.hpp"
#include "engine/Service.hpp"

#include "engine/SceneManager.hpp"
#include "engine/Window.hpp"

class EngineContext
{

public:
    EngineContext() = default;
    ~EngineContext() = default;

    template <ServiceType T>
    void registerService(std::unique_ptr<T> service)
    {
        if (!m_services.emplace(std::type_index(typeid(T)), std::move(service)).second)
        {
            throw std::runtime_error("Service already registered.");
        }
    }

    template <ServiceType T>
    T &getService() const
    {
        auto it = m_services.find(std::type_index(typeid(T)));
        if (it == m_services.end())
        {
            throw std::runtime_error("Service not registered.");
        }
        return *static_cast<T *>(it->second.get());
    }

    Scene &currentScene()
    {
        return getService<SceneManager>().currentScene();
    }

    Registry &registry()
    {
        return currentScene().registry();
    }

    SystemScheduler &systems()
    {
        return currentScene().systems();
    }

    Window &window()
    {
        return getService<Window>();
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<Service>> m_services;
};