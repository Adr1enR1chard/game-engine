#pragma once

#include <any>
#include <memory>
#include <queue>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "utils/types.hpp"
#include <engine/Entity.hpp>
#include <engine/Service.hpp>

template <typename T> struct ComponentStorage {
    bool has(Entity entity) const
    {
        return components.find(entity) != components.end();
    }

    T& emplace(Entity entity, const T& component)
    {
        return components[entity] = component;
    }

    T* get(Entity entity)
    {
        auto it = components.find(entity);
        if (it != components.end()) {
            return &it->second;
        }
        return nullptr;
    }

    std::vector<Entity> getEntities() const
    {
        std::vector<Entity> entities;
        for (const auto& [entity, component] : components) {
            entities.push_back(entity);
        }
        return entities;
    }

  private:
    std::unordered_map<Entity, T> components;
};

class World
{

  public:
    World()                        = default;
    World(const World&)            = delete;
    World& operator=(const World&) = delete;
    World(World&&)                 = default;
    World& operator=(World&&)      = default;
    ~World()                       = default;

    Entity                           createEntity();
    template <typename... Cs> Entity createEntity(Cs&&... components);

    template <typename T> const std::vector<Entity>     getEntitiesWithComponent() const;
    template <typename... Cs> const std::vector<Entity> getEntitiesWithComponents() const;
    template <typename T> T&                            createComponent(Entity entity);
    template <typename T> T&                            createComponent(Entity entity, T&& component);
    template <typename... T> void                       createComponents(Entity entity);
    template <typename... T> void                       createComponents(Entity entity, T&&... components);
    template <typename T> T&                            getComponent(Entity entity);
    template <typename T> bool                          hasComponent(Entity entity) const;

    template <ServiceType T> T& Serv();

  private:
    template <typename T> ComponentStorage<T>&       registry();
    template <typename T> const ComponentStorage<T>* tryRegistry() const;

  private:
    int                 m_entityIndex = 0;
    std::queue<int>     m_freeIndices;
    std::vector<Entity> m_entities;

    std::unordered_map<std::type_index, std::any>                 m_components;
    std::unordered_map<std::type_index, std::unique_ptr<Service>> m_services;
};

template <typename... Cs> Entity World::createEntity(Cs&&... components)
{
    Entity entity = createEntity();
    createComponents<Cs...>(entity, std::forward<Cs>(components)...);
    return entity;
}

template <typename T> const std::vector<Entity> World::getEntitiesWithComponent() const
{
    const ComponentStorage<T>* storage = tryRegistry<T>();
    if (!storage)
        return {};

    return storage->getEntities();
}

template <typename... Cs> const std::vector<Entity> World::getEntitiesWithComponents() const
{
    std::vector<Entity> entitiesWithAllComponents;

    for (const Entity& entity : m_entities) {
        bool hasAll = (hasComponent<Cs>(entity) && ...);
        if (hasAll) {
            entitiesWithAllComponents.push_back(entity);
        }
    }

    return entitiesWithAllComponents;
}

template <typename T> T& World::createComponent(Entity entity)
{
    return registry<T>().emplace(entity, T{});
}

template <typename T> T& World::createComponent(Entity entity, T&& component)
{
    return registry<T>().emplace(entity, std::forward<T>(component));
}

template <typename... T> void World::createComponents(Entity entity)
{
    (createComponent<T>(entity), ...);
}

template <typename... T> void World::createComponents(Entity entity, T&&... components)
{
    (createComponent<T>(entity, std::forward<T>(components)), ...);
}

template <typename T> T& World::getComponent(Entity entity)
{
    auto* ptr = registry<T>().get(entity);
    if (!ptr)
        throw std::runtime_error("Component not found");

    return *ptr;
}

template <typename T> bool World::hasComponent(Entity entity) const
{
    return tryRegistry<T>() && tryRegistry<T>()->has(entity);
}

template <ServiceType T> T& World::Serv()
{
    auto it = m_services.find(std::type_index(typeid(T)));
    if (it == m_services.end()) {
        it = m_services.emplace(std::type_index(typeid(T)), std::make_unique<T>()).first;
    }
    return *static_cast<T*>(it->second.get());
}

template <typename T> ComponentStorage<T>& World::registry()
{
    auto& storage = m_components[std::type_index(typeid(T))];
    if (!storage.has_value())
        storage = ComponentStorage<T>{};

    return std::any_cast<ComponentStorage<T>&>(storage);
}

template <typename T> const ComponentStorage<T>* World::tryRegistry() const
{
    auto it = m_components.find(std::type_index(typeid(T)));
    if (it != m_components.end()) {
        return &std::any_cast<const ComponentStorage<T>&>(it->second);
    }
    return nullptr;
}