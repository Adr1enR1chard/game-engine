#pragma once

#include <concepts>

class Component;

template <typename T>
concept ComponentType = std::derived_from<T, Component>;