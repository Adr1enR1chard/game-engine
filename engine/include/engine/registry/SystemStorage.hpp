
#pragma once

#include <unordered_map>
#include <vector>

#include <engine/model/System.hpp>

struct SystemStorage
{
    template <SystemType T, typename... Args>
    void emplace(Args &&...args)
    {
        auto it = m_systemTypeToIndex.find(std::type_index(typeid(T)));
        if (it != m_systemTypeToIndex.end())
        {
            size_t index = it->second;
            m_systems[index] = std::make_unique<T>(std::forward<Args>(args)...);
        }
        else
        {
            m_systems.push_back(std::make_unique<T>(std::forward<Args>(args)...));
            size_t index = m_systems.size() - 1;
            m_systemTypeToIndex[std::type_index(typeid(T))] = index;
        }
    }

    template <SystemType T>
    T &get()
    {
        auto it = m_systemTypeToIndex.find(std::type_index(typeid(T)));
        if (it != m_systemTypeToIndex.end())
        {
            size_t index = it->second;
            return static_cast<T &>(*m_systems[index]);
        }
        return nullptr;
    }

    template <SystemType T>
    void erase()
    {
        auto it = m_systemTypeToIndex.find(std::type_index(typeid(T)));
        if (it != m_systemTypeToIndex.end())
        {
            size_t index = it->second;
            m_systems.erase(m_systems.begin() + index);
            m_systemTypeToIndex.erase(it);
            // Update indices
            for (auto &pair : m_systemTypeToIndex)
            {
                if (pair.second > index)
                {
                    pair.second--;
                }
            }
        }
    }

    void map(std::function<void(System &)> func)
    {
        for (auto &system : m_systems)
        {
            func(*system);
        }
    }

private:
    std::vector<std::unique_ptr<System>> m_systems;
    std::unordered_map<std::type_index, size_t> m_systemTypeToIndex;
};