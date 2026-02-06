#include "EntitiesManager.hpp"

void EntitiesManager::setEntityName(Entity entity, const std::string &name)
{
    m_entityNames[entity] = name;
}

std::string EntitiesManager::getEntityName(Entity entity) const
{
    return m_entityNames.contains(entity) ? m_entityNames.at(entity) : "Entity " + std::to_string(entity);
}