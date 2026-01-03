#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include <vector>
#include <algorithm>

#include "ecs/entity/Entity.hpp"

class Component
{
private:
    std::vector<Entity> entityIndices;

public:
    /// @brief Indicates whether the component is reserved (cannot be removed from a Entity).
    /// @return
    virtual bool reserved() const { return true; }

    /// @brief Indicates whether the component is unique (only one instance allowed per Entity).
    /// @return
    virtual bool unique() const { return true; }

public:
    Component() {}
    virtual ~Component() = default;

    void bindToEntity(Entity entity)
    {
        entityIndices.push_back(entity);
    }

    void unbindFromEntity(Entity entity)
    {
        entityIndices.erase(std::remove(entityIndices.begin(), entityIndices.end(), entity), entityIndices.end());
    }
};

#endif