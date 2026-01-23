#pragma once

#include <concepts>
#include <typeindex>
#include <vector>

class SystemScheduler;

class Bundle
{
  public:
    virtual ~Bundle()                                                                            = default;
    virtual void install(class SystemRegistry& systems, class ServiceRegistry& services) const   = 0;
    virtual void uninstall(class SystemRegistry& systems, class ServiceRegistry& services) const = 0;
};

template <typename T>
concept BundleType = std::derived_from<T, Bundle>;
