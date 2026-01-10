#include <engine/Texture.hpp>

#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <iostream>

Texture::Texture(const char *imagePath)
{
    int width, height, nrChannels;

    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        // default texture wrapping and filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format = GL_RGB;
        GLenum internalFormat = GL_RGB8;

        if (nrChannels == 1)
        {
            format = GL_RED;
            internalFormat = GL_R8;
        }
        else if (nrChannels == 2)
        {
            format = GL_RG;
            internalFormat = GL_RG8;
        }
        else if (nrChannels == 3)
        {
            format = GL_RGB;
            internalFormat = GL_RGB8;
        }
        else if (nrChannels == 4)
        {
            format = GL_RGBA;
            internalFormat = GL_RGBA8;
        }
        else
        {
            std::cout << "Unexpected channel count (" << nrChannels << ") for " << imagePath << "\n";
            stbi_image_free(data);
            return;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        std::cout << "TextureHandle: Failed to load texture at path: " << imagePath << std::endl;
    }
    stbi_image_free(data);
}

Texture Texture::White()
{
    unsigned int whiteTextureID;
    glGenTextures(1, &whiteTextureID);
    glBindTexture(GL_TEXTURE_2D, whiteTextureID);

    unsigned char whitePixel[4] = {255, 255, 255, 255};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);

    // default texture wrapping and filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return Texture(whiteTextureID);
}

void Texture::filteringParameters(unsigned int minFilter, unsigned int magFilter) const
{
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, ID);
}