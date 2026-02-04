#pragma once

#include <engine/model/Service.hpp>
#include <engine/bundle/standalone/service/Renderer.hpp>
#include <engine/utils/RenderTypes.hpp>

namespace default_bundle
{
    using namespace engine;

    class TextureFactory : public engine::Service
    {
    public:
        TextureFactory(Renderer &renderer) : m_renderer(renderer) {}

        TextureRef Texture2D(std::string imagePath);
        TextureRef CubeMap(const std::vector<std::string> &faces);

        TextureRef WhiteTexture2D()
        {
            return Texture2D("default-bundle-assets/textures/white1x1.png");
        }

        TextureRef BlackTexture2D()
        {
            return Texture2D("default-bundle-assets/textures/black1x1.png");
        }

        TextureRef NormalTexture2D()
        {
            return Texture2D("default-bundle-assets/textures/normal1x1.png");
        }

    private:
        Renderer &m_renderer;
    };

} // namespace default_bundle