#pragma once

#include <any>
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "utils/types.hpp"
#include <engine/Entity.hpp>

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

class Registry
{
  private:
    int                 m_entityIndex = 0;
    std::queue<int>     m_freeIndices;
    std::vector<Entity> m_entities;

    std::unordered_map<std::type_index, std::any> m_componentMap;

  public:
    Registry()                           = default;
    Registry(const Registry&)            = delete;
    Registry& operator=(const Registry&) = delete;
    Registry(Registry&&)                 = default;
    Registry& operator=(Registry&&)      = default;
    ~Registry()                          = default;

    Entity createEntity()
    {
        Entity entity;
        if (!m_freeIndices.empty()) {
            int index = m_freeIndices.front();
            m_freeIndices.pop();
            entity = index;
        } else {
            entity = m_entityIndex;
            m_entityIndex++;
        }

        m_entities.push_back(entity);

        return entity;
    }

    template <typename T> const std::vector<Entity> getEntitiesWithComponent() const
    {
        const ComponentStorage<T>* storage = tryStorage<T>();
        if (!storage)
            return {};

        return storage->getEntities();
    }

    template <typename... Components> const std::vector<Entity> getEntitiesWithComponents() const
    {
        std::vector<Entity> entitiesWithAllComponents;

        for (const Entity& entity : m_entities) {
            bool hasAll = (hasComponent<Components>(entity) && ...);
            if (hasAll) {
                entitiesWithAllComponents.push_back(entity);
            }
        }

        return entitiesWithAllComponents;
    }

    template <typename T> T& createComponent(Entity entity)
    {
        return storage<T>().emplace(entity, T{});
    }

    template <typename... T> void createComponents(Entity entity)
    {
        (createComponent<T>(entity), ...);
    }

    template <typename T> T& getComponent(Entity entity)
    {
        auto* ptr = storage<T>().get(entity);
        if (!ptr)
            std::runtime_error("Component not found");

        return *ptr;
    }

    template <typename T> bool hasComponent(Entity entity) const
    {
        return tryStorage<T>() && tryStorage<T>()->has(entity);
    }

  private:
    template <typename T> ComponentStorage<T>& storage()
    {
        auto& storage = m_componentMap[std::type_index(typeid(T))];
        if (!storage.has_value())
            storage = ComponentStorage<T>{};

        return std::any_cast<ComponentStorage<T>&>(storage);
    }

    template <typename T> const ComponentStorage<T>* tryStorage() const
    {
        auto it = m_componentMap.find(std::type_index(typeid(T)));
        if (it != m_componentMap.end()) {
            return &std::any_cast<const ComponentStorage<T>&>(it->second);
        }
        return nullptr;
    }
};