#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include <vector>
#include <algorithm>

#include "ecs/entity/Entity.hpp"

class Component
{
private:
    std::vector<int> entityIndices;

public:
    /// @brief Indicates whether the component is reserved (cannot be removed from a Entity).
    /// @return
    virtual bool reserved() const { return true; }

    /// @brief Indicates whether the component is unique (only one instance allowed per Entity).
    /// @return
    virtual bool unique() const { return true; }

public:
    Component() {}
    virtual ~Component() {}

    void bindToEntity(int entityIndex)
    {
        entityIndices.push_back(entityIndex);
    }

    void bindToEntity(Entity &entity)
    {
        entityIndices.push_back(entity.getIndex());
    }

    void unbindFromEntity(int entityIndex)
    {
        entityIndices.erase(std::remove(entityIndices.begin(), entityIndices.end(), entityIndex), entityIndices.end());
    }

    void unbindFromEntity(Entity &entity)
    {
        int entityIndex = entity.getIndex();
        entityIndices.erase(std::remove(entityIndices.begin(), entityIndices.end(), entityIndex), entityIndices.end());
    }
};

#endif