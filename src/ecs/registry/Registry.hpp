#pragma once

#include <unordered_map>
#include <vector>
#include <queue>
#include <typeindex>

#include "ecs/entity/Entity.hpp"
#include "ecs/component/Component.hpp"
#include "utils/types.hpp"

struct ComponentStorage
{
    std::unordered_map<Entity, std::unique_ptr<Component>> components;
};

class Registry
{
private:
    int m_entityIndex = 0;
    std::queue<int> m_freeIndices;

    int m_componentTypeIDCounter = 0;
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

        return entity;
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
};