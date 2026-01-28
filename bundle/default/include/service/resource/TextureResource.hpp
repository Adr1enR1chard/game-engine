#pragma once

#include <engine/model/Service.hpp>
#include <memory>
#include <unordered_map>
#include <utils/IdManager.hpp>
#include <utils/RenderTypes.hpp>

class TextureResource : public Service
{
public:
  TextureResource() = default;
  ~TextureResource() override = default;

  TextureRef texture2d(unsigned int width, unsigned int height, unsigned int channels, unsigned char *data);
  TextureRef cubeMap(std::vector<unsigned int> widths, std::vector<unsigned int> heights, std::vector<unsigned int> channels, const std::vector<std::vector<unsigned char>> &facesData);

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