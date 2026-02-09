#include <service/SSR.hpp>

#include <engine/bundle/core/service/Renderer.hpp>
#include <engine/bundle/core/service/factory/ShaderFactory.hpp>

namespace default_rendering
{
    using namespace engine;

    void SSR::initialize(Renderer &renderer, ShaderFactory &shaderFactory)
    {
        m_ssrShader = shaderFactory.CustomShader("__SSRShader",
                                                 "default-bundle-assets/shaders/Image.vert",
                                                 "default-bundle-assets/shaders/ssr/SSR.frag", {});
    }
} // namespace default_rendering