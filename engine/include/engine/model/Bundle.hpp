#pragma once

#include <concepts>
#include <typeindex>
#include <vector>

class SystemScheduler;

class Bundle
{
  public:
    virtual ~Bundle()                               = default;
    virtual void apply(class Engine& engine) const  = 0;
    virtual void remove(class Engine& engine) const = 0;

  private:
    std::vector<std::type_index> m_systems;
};

template <typename T>
concept BundleType = std::derived_from<T, Bundle>;
