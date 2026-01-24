#include <service/resource/TextureResource.hpp>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <engine/utils/Log.hpp>

struct TextureResource::TextureData {
    unsigned int textureID = 0;
};

void TextureResource::TextureDataDeleter::operator()(TextureData* textureData)
{
    if (textureData) {
        glDeleteTextures(1, &textureData->textureID);
        delete textureData;
    }
}

TextureRef TextureResource::create(const char* imagePath)
{
    TextureRef newTextureRef = m_idManager.alloc();
    auto       textureData   = std::unique_ptr<TextureData, TextureDataDeleter>(new TextureData());

    int            width, height, nrChannels;
    unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format         = GL_RGB;
        GLenum internalFormat = GL_RGB8;

        if (nrChannels == 1) {
            format         = GL_RED;
            internalFormat = GL_R8;
        } else if (nrChannels == 2) {
            format         = GL_RG;
            internalFormat = GL_RG8;
        } else if (nrChannels == 3) {
            format         = GL_RGB;
            internalFormat = GL_RGB8;
        } else if (nrChannels == 4) {
            format         = GL_RGBA;
            internalFormat = GL_RGBA8;
        } else {
            stbi_image_free(data);
            m_idManager.free(newTextureRef);
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

        stbi_image_free(data);

        m_textures[newTextureRef] = std::move(textureData);
        return newTextureRef;
    }
    Log::Print("Failed to load texture: " + std::string(imagePath), LogLevel::Critical);
    m_idManager.free(newTextureRef);
    return 0;
}

void TextureResource::remove(TextureRef textureRef)
{
    m_idManager.free(textureRef);
    m_textures.erase(textureRef);
}

void TextureResource::bind(TextureRef textureRef) const
{
    auto it = m_textures.find(textureRef);
    if (it != m_textures.end()) {
        glBindTexture(GL_TEXTURE_2D, it->second->textureID);
    }
}