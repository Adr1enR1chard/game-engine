#pragma once

#include <string>
#include <unordered_map>

#include <engine/model/Entity.hpp>

using namespace engine;

class EntitiesManager
{
public:
    EntitiesManager() = default;
    ~EntitiesManager() = default;

    EntitiesManager(const EntitiesManager &) = delete;
    EntitiesManager &operator=(const EntitiesManager &) = delete;

    void setEntityName(Entity entity, const std::string &name);
    std::string getEntityName(Entity entity) const;

private:
    std::unordered_map<Entity, std::string> m_entityNames;
};