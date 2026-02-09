#pragma once

#include <engine/model/Service.hpp>

#include <engine/utils/RenderTypes.hpp>

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
        SSR() = default;
        ~SSR() override = default;

        void initialize(Renderer &renderer, ShaderFactory &shaderFactory);

        const ShaderRef &getShader() const { return m_ssrShader; }

    private:
        ShaderRef m_ssrShader = 0;
    };

} // namespace default_rendering