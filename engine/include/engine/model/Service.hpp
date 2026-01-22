#pragma once

#include <concepts>

/**
 * Base class for services in the engine. A service is a globally accessible
 * instance that provides specific functionality, such as window management,
 * input handling, or resource management.
 */
class Service
{
  public:
    Service() = default;
    virtual ~Service();
    Service(const Service&)            = delete;
    Service& operator=(const Service&) = delete;
};

template <typename T>
concept ServiceType = std::derived_from<T, Service>;