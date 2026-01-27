#pragma once

#include <engine/model/Service.hpp>
#include <service/resource/TextureResource.hpp>
#include <utils/RenderTypes.hpp>

class TextureFactory : public Service
{
public:
    TextureFactory(TextureResource &textureResource) : m_textureResource(textureResource) {}

    TextureRef Texture2D(const char *imagePath);
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
    TextureResource &m_textureResource;
};