#pragma once

#include <queue>

namespace engine
{

    /// @brief Utility class to manage unique IDs.
    class IdManager
    {
    public:
        IdManager() = default;
        ~IdManager() = default;

        unsigned int alloc()
        {
            if (!m_freeIds.empty())
            {
                unsigned int id = m_freeIds.front();
                m_freeIds.pop();
                return id;
            }
            return m_nextId++;
        }

        void free(unsigned int id)
        {
            m_freeIds.push(id);
        }

    private:
        unsigned int m_nextId = 1;
        std::queue<unsigned int> m_freeIds;
    };

} // namespace engine