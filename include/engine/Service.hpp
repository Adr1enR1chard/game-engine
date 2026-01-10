#pragma once

class Service
{
  public:
    Service()                          = default;
    virtual ~Service()                 = default;
    Service(const Service&)            = delete;
    Service& operator=(const Service&) = delete;
};