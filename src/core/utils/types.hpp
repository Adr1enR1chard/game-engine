#ifndef __TYPES_HPP__
#define __TYPES_HPP__

#include "core/components/component.hpp"
#include <concepts>

template <typename T>
concept ComponentType = std::derived_from<T, Component>;

#endif