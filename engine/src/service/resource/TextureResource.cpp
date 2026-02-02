#include <engine/service/resource/TextureResource.hpp>

#include <glad/glad.h>

#include <engine/utils/Log.hpp>
#include <assets_format/opengl_format.hpp>

namespace engine
{

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

    TextureRef TextureResource::texture2D(const TextureAttributes &texture)
    {
        if (!OpenGLFormat::IsCompressed(texture.format))
        {
            Log::Print("Texture format is not compressed. Use the asset compiler to compress the texture.", LogLevel::Error);
            return 0;
        }

        TextureRef newTextureRef = m_idManager.alloc();
        auto textureData = std::unique_ptr<TextureData, TextureDataDeleter>(new TextureData());
        textureData->type = Texture2D;

        GLuint glFormat = OpenGLFormat::GetOpenGLFormat(
            texture.format,
            texture.channels == 4);

        glGenTextures(1, &textureData->textureID);
        glBindTexture(GL_TEXTURE_2D, textureData->textureID);

        glCompressedTexImage2D(
            GL_TEXTURE_2D,
            0,
            glFormat,
            texture.width,
            texture.height,
            0,
            texture.data.size(),
            texture.data.data());

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        m_textures[newTextureRef] = std::move(textureData);
        return newTextureRef;
    }

    TextureRef TextureResource::cubeMap(const std::vector<TextureAttributes> &facesData)
    {
        TextureRef newTextureRef = m_idManager.alloc();
        auto textureData = std::unique_ptr<TextureData, TextureDataDeleter>(new TextureData());
        textureData->type = CubeMap;

        glGenTextures(1, &textureData->textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureData->textureID);

        for (unsigned int i = 0; i < facesData.size(); i++)
        {
            if (!OpenGLFormat::IsCompressed(facesData[i].format))
            {
                Log::Print("CubeMap face format is not compressed. Use the asset compiler to compress the texture.", LogLevel::Error);
                return 0;
            }

            GLuint glFormat = OpenGLFormat::GetOpenGLFormat(
                facesData[i].format,
                facesData[i].channels == 4);

            glCompressedTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                glFormat,
                facesData[i].width,
                facesData[i].height,
                0,
                facesData[i].data.size(),
                facesData[i].data.data());
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        m_textures[newTextureRef] = std::move(textureData);
        return newTextureRef;
    }

    TextureRef TextureResource::depthMap(const unsigned int width, const unsigned int height)
    {
        TextureRef newTextureRef = m_idManager.alloc();
        auto textureData = std::unique_ptr<TextureData, TextureDataDeleter>(new TextureData());
        textureData->type = Texture2D;
        unsigned int depthMap;
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = {1.0, 1.0, 1.0, 1.0};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        textureData->textureID = depthMap;

        // Attach depth texture as FBO's depth buffer, this requires binding the FBO before calling this method
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

        Log::Print("Created depth map texture with ID: " + std::to_string(depthMap) + "(" + std::to_string(width) + "x" + std::to_string(height) + ")", LogLevel::Debug);

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

} // namespace engine