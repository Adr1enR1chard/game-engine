#pragma once

#include <engine/model/Service.hpp>
#include <service/resource/TextureResource.hpp>
#include <utils/RenderTypes.hpp>

class TextureFactory : public Service
{
public:
    TextureFactory(TextureResource &texture2DResource) : m_texture2DResource(texture2DResource) {}

    TextureRef Texture2D(const char *imagePath)
    {
        return m_texture2DResource.texture2D(imagePath);
    }

    TextureRef WhiteTexture2D()
    {
        return m_texture2DResource.texture2D("default-bundle-assets/textures/white1x1.png");
    }

    TextureRef BlackTexture2D()
    {
        return m_texture2DResource.texture2D("default-bundle-assets/textures/black1x1.png");
    }

    TextureRef NormalTexture2D()
    {
        return m_texture2DResource.texture2D("default-bundle-assets/textures/normal1x1.png");
    }

    TextureRef CubeMap(const std::vector<std::string> &faces)
    {
        return m_texture2DResource.cubeMap(faces);
    }

private:
    TextureResource &m_texture2DResource;
};