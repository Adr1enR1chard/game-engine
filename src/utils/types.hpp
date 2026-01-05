#pragma once

#include <concepts>

#include "engine/Service.hpp"
#include "ecs/component/CComponent.hpp"
#include "ecs/system/System.hpp"

template <typename T>
concept ComponentType = std::derived_from<T, CComponent>;

template <typename T>
concept SystemType = std::derived_from<T, System>;

template <typename T>
concept ServiceType = std::derived_from<T, Service>;