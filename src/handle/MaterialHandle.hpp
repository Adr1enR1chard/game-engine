#pragma once
#include <vector>
#include <string>
#include "ShaderHandle.hpp"
#include "TextureHandle.hpp"

class MaterialHandle
{
public:
    MaterialHandle(const char *vertexPath, const char *fragmentPath) : shader(vertexPath, fragmentPath) {};
    ~MaterialHandle() = default;

    TextureHandle &addTexture(const TextureHandle &texture, std::string name = "")
    {
        if (textures.size() >= 32)
        {
            throw std::runtime_error("MaterialHandle can only hold up to 32 textures.");
        }

        textures.push_back(texture);
        std::string textureName = name.empty() ? "texture" + std::to_string(textures.size() - 1) : name;
        textureNames.push_back(textureName);

        return textures.back();
    }

    ShaderHandle &setShader(const ShaderHandle &shaderHandle)
    {
        shader = shaderHandle;
        return shader;
    }

    const ShaderHandle &getShader() const
    {
        return shader;
    }

    const std::vector<TextureHandle> &getTextures() const
    {
        return textures;
    }

    const std::vector<std::string> &getTextureNames() const
    {
        return textureNames;
    }

private:
    std::vector<TextureHandle> textures;
    std::vector<std::string> textureNames;
    ShaderHandle shader;
};