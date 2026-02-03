#pragma once

#include <engine/model/Service.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include <engine/utils/IdManager.hpp>
#include <engine/utils/RenderTypes.hpp>

#include <assets_format/texture_format.hpp>

namespace engine
{

  struct TextureAttributes
  {
    unsigned int width;
    unsigned int height;
    unsigned int channels;
    TextureFormat format;
    std::vector<unsigned char> data;
  };

  class TextureResource : public Service
  {
  public:
    TextureResource() = default;
    ~TextureResource() override = default;

    TextureRef texture2D(const TextureAttributes &texture);
    TextureRef cubeMap(const std::vector<TextureAttributes> &faces);
    TextureRef depthMap(const unsigned int width, const unsigned int height);

    void remove(TextureRef textureRef);
    void bind(TextureRef textureRef) const;

  private:
    struct TextureData;

    struct TextureDataDeleter
    {
      void operator()(TextureData *textureData);
    };

    IdManager m_idManager;
    std::unordered_map<TextureRef, std::unique_ptr<TextureData, TextureDataDeleter>> m_textures;
  };

} // namespace engine