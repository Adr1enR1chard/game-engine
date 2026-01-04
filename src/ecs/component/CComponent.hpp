#pragma once

struct CComponent
{
    virtual ~CComponent() = default;
    inline virtual const bool isValid() const = 0;
};