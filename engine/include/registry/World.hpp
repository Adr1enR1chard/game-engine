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

class World
{

  public:
    World()                        = default;
    World(const World&)            = delete;
    World& operator=(const World&) = delete;
    World(World&&)                 = default;
    World& operator=(World&&)      = default;
    ~World()                       = default;

    template <typename... Cs> Entity create(Cs&&... components);

    /**
     * Retrieve entities and their components matching the query.
     * Usage: for (auto [e,a,b] : world.get<A,B>()) { ... }
     *
     * @return Vector of tuples; empty if no match.
     */
    template <typename... Cs> std::vector<std::tuple<Entity, Cs*...>> get();

    /**
     * Retrieve components for a specific entity.
     * Usage: auto [e,a,b] = world.getFrom<A,B>(entity);
     *
     * @warning If the entity does not have all requested components, entity will be 0 and pointers will be nullptr.
     */
    template <typename... Cs> std::tuple<Entity, Cs*...> getFrom(Entity entity);

    /**
     * Retrieve entities and their components matching the query at a specific index.
     * Usage: auto [e,a,b] = world.getAt<A,B>(0);
     *
     * @warning If the index is out of range, entity will be 0 and pointers will be nullptr.
     */
    template <typename... Cs> std::tuple<Entity, Cs*...> getAt(size_t index);

    /**
     * Add components to an entity.
     */
    template <typename... Cs> void add(Entity entity, Cs&&... components);

    /**
     * Check if an entity has all specified components.
     */
    template <typename... Cs> bool has(Entity entity) const;

  private:
    Entity                                           allocateEntity();
    template <typename T> ComponentStorage<T>&       registry();
    template <typename T> const ComponentStorage<T>* tryRegistry() const;

  private:
    int                 m_entityIndex = 1;
    std::queue<int>     m_freeIndices;
    std::vector<Entity> m_entities;

    std::unordered_map<std::type_index, std::any> m_components;
};

template <typename... Cs> Entity World::create(Cs&&... components)
{
    Entity entity = allocateEntity();
    add<Cs...>(entity, std::forward<Cs>(components)...);
    return entity;
}

template <typename... Cs> std::vector<std::tuple<Entity, Cs*...>> World::get()
{
    std::vector<std::tuple<Entity, Cs*...>> result;

    for (const Entity& entity : m_entities) {
        if ((has<Cs>(entity) && ...)) {
            result.emplace_back(entity, registry<Cs>().get(entity)...);
        }
    }

    return result;
}

template <typename... Cs> std::tuple<Entity, Cs*...> World::getFrom(Entity entity)
{
    if (!(has<Cs>(entity) && ...)) {
        return {};
    }
    return std::make_tuple(entity, registry<Cs>().get(entity)...);
}

template <typename... Cs> std::tuple<Entity, Cs*...> World::getAt(size_t index)
{
    std::vector<std::tuple<Entity, Cs*...>> result = get<Cs...>();

    if (index < result.size()) {
        return result[index];
    } else {
        return {};
    }
}

template <typename... Cs> void World::add(Entity entity, Cs&&... components)
{
    (registry<Cs>().emplace(entity, std::forward<Cs>(components)), ...);
}

template <typename... Cs> bool World::has(Entity entity) const
{
    return (([&] {
                auto reg = tryRegistry<Cs>();
                return reg && reg->has(entity);
            }()) &&
            ...);
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