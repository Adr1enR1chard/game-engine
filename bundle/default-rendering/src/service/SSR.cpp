#include <service/SSR.hpp>

#include <engine/bundle/core/service/Renderer.hpp>
#include <engine/bundle/core/service/factory/ShaderFactory.hpp>

namespace default_rendering
{
    using namespace engine;

    SSR::SSR(Renderer &renderer, ShaderFactory &shaderFactory)
        : Service()
    {
    }

    SSR::~SSR()
    {
    }
} // namespace default_rendering