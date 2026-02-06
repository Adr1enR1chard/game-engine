#pragma once

#include <concepts>
#include <typeindex>
#include <vector>

namespace engine
{

  class Bundle
  {
  public:
    virtual ~Bundle() = default;
    virtual void install(class Engine &engine) const = 0;
    virtual void uninstall(class Engine &engine) const = 0;
  };

  template <typename T>
  concept BundleType = std::derived_from<T, Bundle>;

} // namespace engine
