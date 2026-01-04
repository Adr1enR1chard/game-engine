#pragma once

#include <concepts>

#include "ecs/component/CComponent.hpp"
#include "ecs/system/System.hpp"

template <typename T>
concept ComponentType = std::derived_from<T, CComponent>;

template <typename T>
concept SystemType = std::derived_from<T, System>;