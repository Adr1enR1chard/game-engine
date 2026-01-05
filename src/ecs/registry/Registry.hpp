#pragma once

#include <unordered_map>
#include <vector>
#include <queue>
#include <typeindex>
#include <memory>
#include <stdexcept>
#include <string>

#include "ecs/entity/Entity.hpp"
#include "ecs/component/CComponent.hpp"
#include "utils/types.hpp"

struct ComponentStorage
{
    std::unordered_map<Entity, std::unique_ptr<CComponent>> components;
};

class Registry
{
private:
    int m_entityIndex = 0;
    std::queue<int> m_freeIndices;
    std::vector<Entity> m_entities;

    std::unordered_map<std::type_index, ComponentStorage> m_componentMap;

public:
    Registry() = default;
    Registry(const Registry &) = delete;
    Registry &operator=(const Registry &) = delete;
    Registry(Registry &&) = default;
    Registry &operator=(Registry &&) = default;
    ~Registry() = default;

    Entity createEntity()
    {
        Entity entity;
        if (!m_freeIndices.empty())
        {
            int index = m_freeIndices.front();
            m_freeIndices.pop();
            entity = index;
        }
        else
        {
            entity = m_entityIndex;
            m_entityIndex++;
        }

        m_entities.push_back(entity);

        return entity;
    }

    const std::vector<Entity> &getEntities() const
    {
        return m_entities;
    }

    template <ComponentType T>
    const std::vector<Entity> getEntitiesWithComponent() const
    {
        std::vector<Entity> entitiesWithComponent;
        const std::type_index typeIndex = std::type_index(typeid(T));

        auto itStorage = m_componentMap.find(typeIndex);
        if (itStorage == m_componentMap.end())
            return entitiesWithComponent;

        const auto &storage = itStorage->second;
        for (const auto &[entity, component] : storage.components)
        {
            entitiesWithComponent.push_back(entity);
        }

        return entitiesWithComponent;
    }

    template <ComponentType... Components>
    const std::vector<Entity> getEntitiesWithComponents() const
    {
        std::vector<Entity> entitiesWithAllComponents;

        for (const Entity &entity : m_entities)
        {
            bool hasAllComponents = true;
            ((hasAllComponents &= (m_componentMap.find(std::type_index(typeid(Components))) != m_componentMap.end() &&
                                   m_componentMap.at(std::type_index(typeid(Components))).components.find(entity) !=
                                       m_componentMap.at(std::type_index(typeid(Components))).components.end())),
             ...);

            if (hasAllComponents)
            {
                entitiesWithAllComponents.push_back(entity);
            }
        }

        return entitiesWithAllComponents;
    }

    template <ComponentType T>
    void registerComponent()
    {
        const std::type_index typeIndex = std::type_index(typeid(T));

        auto [it, inserted] = m_componentMap.try_emplace(typeIndex, ComponentStorage{});
        if (!inserted)
        {
            throw std::runtime_error("Component " + std::string(typeid(T).name()) + " is already registered.");
        }
    }

    template <ComponentType T>
    T &createComponent(Entity entity)
    {
        const std::type_index typeIndex{typeid(T)};

        auto itStorage = m_componentMap.find(typeIndex);
        if (itStorage == m_componentMap.end())
            throw std::runtime_error("Component type not registered.");

        auto &storage = itStorage->second;

        auto [itComp, inserted] = storage.components.emplace(entity, std::make_unique<T>());
        if (!inserted)
            throw std::runtime_error("Entity already has component of this type.");

        return *static_cast<T *>(itComp->second.get());
    }

    template <ComponentType T>
    T &getComponent(Entity entity)
    {
        auto storageIt = m_componentMap.find(std::type_index(typeid(T)));
        if (storageIt == m_componentMap.end())
            throw std::runtime_error("Component type not registered.");

        auto &storage = storageIt->second;
        auto compIt = storage.components.find(entity);
        if (compIt == storage.components.end())
        {
            throw std::runtime_error("Entity does not have component of this type.");
        }

        return *static_cast<T *>(compIt->second.get());
    }

    template <ComponentType T>
    bool hasComponent(Entity entity) const
    {
        auto storageIt = m_componentMap.find(std::type_index(typeid(T)));
        if (storageIt == m_componentMap.end())
            return false;

        const auto &storage = storageIt->second;
        return storage.components.find(entity) != storage.components.end();
    }
};