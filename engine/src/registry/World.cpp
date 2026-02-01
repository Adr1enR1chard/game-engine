#include <engine/registry/World.hpp>

namespace engine
{

    Entity World::allocateEntity()
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

} // namespace engine