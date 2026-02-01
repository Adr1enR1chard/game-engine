#pragma once

#include <engine/model/Service.hpp>
#include <engine/service/resource/TextureResource.hpp>
#include <engine/utils/RenderTypes.hpp>

namespace default_bundle
{
    class TextureFactory : public engine::Service
    {
    public:
        TextureFactory(engine::TextureResource &textureResource) : m_textureResource(textureResource) {}

        engine::TextureRef Texture2D(std::string imagePath);
        engine::TextureRef CubeMap(const std::vector<std::string> &faces);

        engine::TextureRef WhiteTexture2D()
        {
            return Texture2D("default-bundle-assets/textures/white1x1.png");
        }

        engine::TextureRef BlackTexture2D()
        {
            return Texture2D("default-bundle-assets/textures/black1x1.png");
        }

        engine::TextureRef NormalTexture2D()
        {
            return Texture2D("default-bundle-assets/textures/normal1x1.png");
        }

    private:
        engine::TextureResource &m_textureResource;
    };

} // namespace default_bundle