#include <service/resource/TextureResource.hpp>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <engine/utils/Log.hpp>

struct TextureResource::TextureData
{
    unsigned int textureID = 0;
    TextureType type = Texture2D;
};

void TextureResource::TextureDataDeleter::operator()(TextureData *textureData)
{
    if (textureData)
    {
        glDeleteTextures(1, &textureData->textureID);
        delete textureData;
    }
}

TextureRef TextureResource::texture2d(unsigned int width, unsigned int height, unsigned int channels, unsigned char *data)
{
    TextureRef newTextureRef = m_idManager.alloc();
    auto textureData = std::unique_ptr<TextureData, TextureDataDeleter>(new TextureData());
    textureData->type = Texture2D;

    GLenum format = GL_RGB;
    GLenum internalFormat = GL_RGB8;

    if (channels == 1)
    {
        format = GL_RED;
        internalFormat = GL_R8;
    }
    else if (channels == 2)
    {
        format = GL_RG;
        internalFormat = GL_RG8;
    }
    else if (channels == 3)
    {
        format = GL_RGB;
        internalFormat = GL_RGB8;
    }
    else if (channels == 4)
    {
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
    }
    else
    {
        m_idManager.free(newTextureRef);
        Log::Print("Unsupported number of channels for Texture2D: " + std::to_string(channels),
                   LogLevel::Critical);
        return 0;
    }

    glGenTextures(1, &textureData->textureID);
    glBindTexture(GL_TEXTURE_2D, textureData->textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_textures[newTextureRef] = std::move(textureData);
    return newTextureRef;
}

TextureRef TextureResource::cubeMap(std::vector<unsigned int> widths, std::vector<unsigned int> heights, std::vector<unsigned int> channels, const std::vector<unsigned char *> &facesData)
{
    TextureRef newTextureRef = m_idManager.alloc();
    auto textureData = std::unique_ptr<TextureData, TextureDataDeleter>(new TextureData());
    textureData->type = CubeMap;

    glGenTextures(1, &textureData->textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureData->textureID);

    for (unsigned int i = 0; i < facesData.size(); i++)
    {
        GLenum format;
        if (channels[i] == 1)
        {
            format = GL_RED;
        }
        else if (channels[i] == 3)
        {
            format = GL_RGB;
        }
        else if (channels[i] == 4)
        {
            format = GL_RGBA;
        }
        else
        {
            m_idManager.free(newTextureRef);
            Log::Print("Unsupported number of channels for CubeMap face: " + std::to_string(channels[i]),
                       LogLevel::Critical);
            return 0;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, widths[i], heights[i], 0, format, GL_UNSIGNED_BYTE,
                     facesData[i]);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_textures[newTextureRef] = std::move(textureData);
    return newTextureRef;
}

void TextureResource::remove(TextureRef textureRef)
{
    m_idManager.free(textureRef);
    m_textures.erase(textureRef);
}

void TextureResource::bind(TextureRef textureRef) const
{
    auto it = m_textures.find(textureRef);
    if (it != m_textures.end())
    {
        glBindTexture(it->second->type == Texture2D ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP, it->second->textureID);
    }
}