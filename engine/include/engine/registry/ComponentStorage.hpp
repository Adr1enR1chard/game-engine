
#pragma once

#include <unordered_map>
#include <vector>

#include <engine/model/Entity.hpp>

namespace engine
{

    template <typename T>
    struct ComponentStorage
    {
        bool has(Entity entity) const
        {
            return m_entityToIndex.find(entity) != m_entityToIndex.end();
        }

        T &emplace(Entity entity, const T &component)
        {
            auto it = m_entityToIndex.find(entity);
            if (it != m_entityToIndex.end())
            {
                // Component already exists, update it
                size_t index = it->second;
                m_componentsList[index] = component;
                return m_componentsList[index];
            }
            else
            {
                // New component
                m_componentsList.push_back(component);
                size_t index = m_componentsList.size() - 1;
                m_entityToIndex[entity] = index;
                m_entities.push_back(entity);
                return m_componentsList[index];
            }
        }

        T *get(Entity entity)
        {
            auto it = m_entityToIndex.find(entity);
            if (it != m_entityToIndex.end())
            {
                size_t index = it->second;
                return &m_componentsList[index];
            }
            return nullptr;
        }

        void remove(Entity entity)
        {
            auto it = m_entityToIndex.find(entity);
            if (it != m_entityToIndex.end())
            {
                size_t index = it->second;
                m_componentsList.erase(m_componentsList.begin() + index);
                m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
                m_entityToIndex.erase(it);

                // Update indices for entities after the removed one
                for (size_t i = index; i < m_componentsList.size(); ++i)
                {
                    Entity e = m_entities[i];
                    m_entityToIndex[e] = i;
                }
            }
        }

        const std::vector<Entity> &getEntities() const
        {
            return m_entities;
        }

    private:
        std::vector<Entity> m_entities;
        std::vector<T> m_componentsList;
        std::unordered_map<Entity, size_t> m_entityToIndex;
    };

} // namespace engine