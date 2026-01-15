#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include <engine/model/Service.hpp>

#include <engine/utils/types.hpp>

class ServiceRegistry
{
  public:
    ServiceRegistry()                                  = default;
    ~ServiceRegistry()                                 = default;
    ServiceRegistry(const ServiceRegistry&)            = delete;
    ServiceRegistry& operator=(const ServiceRegistry&) = delete;

    /**
     * Add services to the registry.
     */
    template <ServiceType... T> void add()
    {
        (m_services.try_emplace(std::type_index(typeid(T)), std::make_unique<T>()), ...);
    }

    /**
     * Remove services from the registry.
     */
    template <ServiceType... T> void remove()
    {
        (m_services.erase(std::type_index(typeid(T))), ...);
    }

    template <ServiceType T> T* get()
    {
        auto it = m_services.find(std::type_index(typeid(T)));
        if (it != m_services.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

  private:
    std::unordered_map<std::type_index, std::unique_ptr<Service>> m_services;
};