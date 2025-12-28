#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include "core/game_objects/game_object.hpp"

class Component
{
public:
    /// @brief Indicates whether the component is reserved (cannot be removed from a GameObject).
    /// @return
    virtual bool reserved() const { return true; }

    /// @brief Indicates whether the component is unique (only one instance allowed per GameObject).
    /// @return
    virtual bool unique() const { return true; }

public:
    Component() {}
    virtual ~Component() {}
};

#endif