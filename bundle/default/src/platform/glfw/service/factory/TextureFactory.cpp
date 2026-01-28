#include <service/factory/TextureFactory.hpp>

#include <utils/AssetsLoader.hpp>
#include <engine/utils/Log.hpp>

TextureRef TextureFactory::Texture2D(std::string imagePath)
{
    unsigned int width, height, channels;
    std::vector<unsigned char> data;
    bool loaded = AssetsLoader::LoadTextureAsset(imagePath, width, height, channels, data);
    if (loaded)
    {
        TextureRef textureRef = m_textureResource.texture2d(width, height, channels, data.data());
        return textureRef;
    }
    Log::Print("Failed to load texture asset: " + imagePath, LogLevel::Critical);
    return 0;
}

TextureRef TextureFactory::CubeMap(const std::vector<std::string> &faces)
{
    std::vector<unsigned int> widths;
    std::vector<unsigned int> heights;
    std::vector<unsigned int> channels;
    std::vector<std::vector<unsigned char>> facesData;

    for (const auto &face : faces)
    {
        unsigned int width, height, channel;
        std::vector<unsigned char> data;
        bool loaded = AssetsLoader::LoadTextureAsset(face, width, height, channel, data);
        if (!loaded)
        {
            Log::Print("Failed to load texture asset: " + face, LogLevel::Critical);
            return 0;
        }

        widths.push_back(width);
        heights.push_back(height);
        channels.push_back(channel);
        facesData.push_back(std::move(data));
    }

    TextureRef textureRef = m_textureResource.cubeMap(widths, heights, channels, facesData);

    return textureRef;
}