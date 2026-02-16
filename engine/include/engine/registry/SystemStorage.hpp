
#pragma once

#include <unordered_map>
#include <vector>

#include <engine/model/System.hpp>

namespace engine
{

    struct SystemStorage
    {
        template <SystemType T, typename... Args>
        void emplace(Args &&...args)
        {
            auto it = m_systemTypeToIndex.find(std::type_index(typeid(T)));
            if (it != m_systemTypeToIndex.end())
            {
                size_t index = it->second;
                m_systems[index] = std::make_shared<T>(std::forward<Args>(args)...);
            }
            else
            {
                m_systems.push_back(std::make_shared<T>(std::forward<Args>(args)...));
                size_t index = m_systems.size() - 1;
                m_systemTypeToIndex[std::type_index(typeid(T))] = index;
            }
        }

        template <SystemType T>
        std::shared_ptr<T> get()
        {
            auto it = m_systemTypeToIndex.find(std::type_index(typeid(T)));
            if (it != m_systemTypeToIndex.end())
            {
                size_t index = it->second;
                return std::static_pointer_cast<T>(m_systems[index]);
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
                // We need to convert index to difference_type for erase
                m_systems.erase(m_systems.begin() + static_cast<std::ptrdiff_t>(index));
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
        std::vector<std::shared_ptr<System>> m_systems;
        std::unordered_map<std::type_index, size_t> m_systemTypeToIndex;
    };

} // namespace engine