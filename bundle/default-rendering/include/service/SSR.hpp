#pragma once

#include <engine/model/Service.hpp>

namespace engine
{
    class Renderer;
    class ShaderFactory;
}

namespace default_rendering
{
    using namespace engine;
    class SSR : public Service
    {
    public:
        SSR(Renderer &renderer, ShaderFactory &shaderFactory);
        ~SSR() override;
    };

} // namespace default_rendering