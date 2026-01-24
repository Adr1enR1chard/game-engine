#pragma once

#include <engine/model/Service.hpp>
#include <memory>
#include <unordered_map>
#include <utils/IdManager.hpp>
#include <utils/RenderTypes.hpp>

class TextureResource : public Service
{
  public:
    TextureResource()           = default;
    ~TextureResource() override = default;

    TextureRef texture2D(const char* imagePath);
    TextureRef cubeMap(const std::vector<std::string>& faces);

    void remove(TextureRef textureRef);
    void bind(TextureRef textureRef) const;

  private:
    struct TextureData;

    struct TextureDataDeleter {
        void operator()(TextureData* textureData);
    };

    IdManager                                                                        m_idManager;
    std::unordered_map<TextureRef, std::unique_ptr<TextureData, TextureDataDeleter>> m_textures;
};