#ifndef __TEXTURE_H_
#define __TEXTURE_H_

#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <iostream>

class TextureHandle
{
public:
    unsigned int ID;
    TextureHandle(const char *imagePath)
    {
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
        if (data)
        {
            glGenTextures(1, &ID);
            glBindTexture(GL_TEXTURE_2D, ID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    void filteringParameters(GLenum minFilter, GLenum magFilter)
    {
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    }

    void bind()
    {
        glBindTexture(GL_TEXTURE_2D, ID);
    }
};

#endif