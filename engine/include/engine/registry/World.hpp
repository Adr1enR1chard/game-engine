#pragma once

#include <any>
#include <memory>
#include <queue>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include <engine/model/Entity.hpp>
#include <engine/model/Service.hpp>
#include <engine/registry/ComponentStorage.hpp>
#include <engine/utils/types.hpp>
#include <optional>

namespace engine
{

    class World
    {

    public:
        World() = default;
        World(const World &) = delete;
        World &operator=(const World &) = delete;
        World(World &&) = default;
        World &operator=(World &&) = default;
        ~World() = default;

        template <typename... Cs>
        Entity createEntity(Cs &&...components);

        /**
         * Retrieve entities and their components matching the query.
         * Usage: for (auto [e,a,b] : world.fetch<A,B>()) { ... }
         *
         * @return Vector of tuples; empty if no match.
         */
        template <typename... Cs>
        std::vector<std::tuple<Entity, Cs *...>> fetch();

        /**
         * Retrieve components for a specific entity.
         * Usage: auto [e,a,b] = world.fetchFrom<A,B>(entity);
         *
         * @warning If the entity does not have all requested components, entity will be 0 and pointers will be nullptr.
         */
        template <typename... Cs>
        std::tuple<Entity, Cs *...> fetchFrom(Entity entity);

        /**
         * Retrieve entities and their components matching the query at a specific index.
         * Usage: auto [e,a,b] = world.fetchAt<A,B>(0);
         *
         * @warning If the index is out of range, entity will be 0 and pointers will be nullptr.
         */
        template <typename... Cs>
        std::tuple<Entity, Cs *...> fetchAt(size_t index);

        /**
         * Add components to an entity.
         */
        template <typename... Cs>
        void addComponents(Entity entity, Cs &&...components);

        /**
         * Check if an entity has all specified components.
         */
        template <typename... Cs>
        bool hasComponents(Entity entity) const;

    private:
        Entity allocateEntity();
        template <typename T>
        ComponentStorage<T> &registry();
        template <typename T>
        const ComponentStorage<T> *tryRegistry() const;

    private:
        int m_entityIndex = 1;
        std::queue<int> m_freeIndices;
        std::vector<Entity> m_entities;

        std::unordered_map<std::type_index, std::any> m_components;
    };

    template <typename... Cs>
    Entity World::createEntity(Cs &&...components)
    {
        Entity entity = allocateEntity();
        addComponents<Cs...>(entity, std::forward<Cs>(components)...);
        return entity;
    }

    template <typename... Cs>
    std::vector<std::tuple<Entity, Cs *...>> World::fetch()
    {
        std::vector<std::tuple<Entity, Cs *...>> result;

        for (const Entity &entity : m_entities)
        {
            if ((hasComponents<Cs>(entity) && ...))
            {
                result.emplace_back(entity, registry<Cs>().get(entity)...);
            }
        }

        return result;
    }

    template <typename... Cs>
    std::tuple<Entity, Cs *...> World::fetchFrom(Entity entity)
    {
        if (!(hasComponents<Cs>(entity) && ...))
        {
            return {};
        }
        return std::make_tuple(entity, registry<Cs>().get(entity)...);
    }

    template <typename... Cs>
    std::tuple<Entity, Cs *...> World::fetchAt(size_t index)
    {
        std::vector<std::tuple<Entity, Cs *...>> result = fetch<Cs...>();

        if (index < result.size())
        {
            return result[index];
        }
        else
        {
            return {};
        }
    }

    template <typename... Cs>
    void World::addComponents(Entity entity, Cs &&...components)
    {
        (registry<Cs>().emplace(entity, std::forward<Cs>(components)), ...);
    }

    template <typename... Cs>
    bool World::hasComponents(Entity entity) const
    {
        return (([&]
                 {
                auto reg = tryRegistry<Cs>();
                return reg && reg->has(entity); }()) &&
                ...);
    }

    template <typename T>
    ComponentStorage<T> &World::registry()
    {
        auto &storage = m_components[std::type_index(typeid(T))];
        if (!storage.has_value())
            storage = ComponentStorage<T>{};

        return std::any_cast<ComponentStorage<T> &>(storage);
    }

    template <typename T>
    const ComponentStorage<T> *World::tryRegistry() const
    {
        auto it = m_components.find(std::type_index(typeid(T)));
        if (it != m_components.end())
        {
            return &std::any_cast<const ComponentStorage<T> &>(it->second);
        }
        return nullptr;
    }

} // namespace engine