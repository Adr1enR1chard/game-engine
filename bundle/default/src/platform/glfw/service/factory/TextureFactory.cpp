#include <service/factory/TextureFactory.hpp>

#include <assets_loader/texture_loader.hpp>

#include <engine/utils/Log.hpp>

namespace default_bundle
{

    using namespace engine;

    TextureRef TextureFactory::Texture2D(std::string imagePath)
    {
        LoadedTexture texture;
        bool loaded = TextureLoader::LoadTexture(imagePath.c_str(), texture);
        if (loaded)
        {
            TextureRef textureRef = m_renderer.allocateTexture2D({
                .width = texture.width,
                .height = texture.height,
                .channels = texture.channels,
                .format = texture.format,
                .data = std::move(texture.data),
            });
            return textureRef;
        }
        Log::Print("Failed to load texture asset: " + imagePath, LogLevel::Critical);
        return 0;
    }

    TextureRef TextureFactory::CubeMap(const std::vector<std::string> &faces)
    {
        std::vector<Renderer::TextureAttributes> textures;
        for (const auto &face : faces)
        {
            std::vector<unsigned char> data;
            LoadedTexture texture;
            bool loaded = TextureLoader::LoadTexture(face.c_str(), texture);
            if (!loaded)
            {
                Log::Print("Failed to load texture asset: " + face, LogLevel::Critical);
                return 0;
            }
            textures.push_back({
                .width = texture.width,
                .height = texture.height,
                .channels = texture.channels,
                .format = texture.format,
                .data = std::move(texture.data),
            });
        }

        TextureRef textureRef = m_renderer.allocateCubeMap(textures);

        return textureRef;
    }
} // namespace default_bundle
