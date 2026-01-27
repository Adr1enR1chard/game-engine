#include <service/factory/TextureFactory.hpp>

#include <stb_image/stb_image.h>

TextureRef TextureFactory::Texture2D(const char *imagePath)
{
    int width, height, nrChannels;
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        TextureRef textureRef = m_textureResource.texture2d(width, height, nrChannels, data);
        stbi_image_free(data);
        return textureRef;
    }
    return 0;
}

TextureRef TextureFactory::CubeMap(const std::vector<std::string> &faces)
{
    std::vector<unsigned int> widths;
    std::vector<unsigned int> heights;
    std::vector<unsigned int> channels;
    std::vector<unsigned char *> facesData;

    for (const auto &face : faces)
    {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(face.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            widths.push_back(static_cast<unsigned int>(width));
            heights.push_back(static_cast<unsigned int>(height));
            channels.push_back(static_cast<unsigned int>(nrChannels));
            facesData.push_back(data);
        }
        else
        {
            for (auto &loadedData : facesData)
            {
                stbi_image_free(loadedData);
            }
            return 0;
        }
    }

    TextureRef textureRef = m_textureResource.cubeMap(widths, heights, channels, facesData);

    for (auto &loadedData : facesData)
    {
        stbi_image_free(loadedData);
    }

    return textureRef;
}