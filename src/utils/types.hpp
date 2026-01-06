#pragma once

#include <concepts>

#include <engine/System.hpp>
#include <engine/Service.hpp>

template <typename T>
concept SystemType = std::derived_from<T, System>;

template <typename T>
concept ServiceType = std::derived_from<T, Service>;