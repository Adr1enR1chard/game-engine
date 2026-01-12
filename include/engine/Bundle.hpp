#pragma once

#include <typeindex>
#include <vector>

class SystemScheduler;

class Bundle
{
  public:
    virtual ~Bundle()                                          = default;
    virtual void apply(class SystemScheduler& scheduler) const = 0;

  private:
    std::vector<std::type_index> m_systems;
};

class DefaultBundle : public Bundle
{
  public:
    void apply(SystemScheduler& scheduler) const override;
};

#include <concepts>

template <typename T>
concept BundleType = std::derived_from<T, Bundle>;
