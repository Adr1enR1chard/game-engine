#pragma once

#include <engine/model/Service.hpp>
#include <service/resource/TextureResource.hpp>
#include <utils/RenderTypes.hpp>

class TextureFactory : public Service
{
  public:
    TextureFactory(TextureResource& texture2DResource) : m_texture2DResource(texture2DResource) {}

    TextureRef Texture2D(const char* imagePath)
    {
        return m_texture2DResource.create(imagePath);
    }

  private:
    TextureResource& m_texture2DResource;
};