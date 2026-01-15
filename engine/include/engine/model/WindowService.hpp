#pragma once

#include <engine/model/Service.hpp>

class WindowService : public Service
{
  public:
    virtual ~WindowService() = default;

    virtual void init(int width, int height, const char* title, bool fullscreen) = 0;
    virtual bool shouldClose() const                                             = 0;
    virtual void swapBuffers()                                                   = 0;
    virtual void pollEvents()                                                    = 0;
    virtual void clear()                                                         = 0;
};