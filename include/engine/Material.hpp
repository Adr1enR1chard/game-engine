#pragma once
#include <vector>
#include <string>

#include <engine/Shader.hpp>
#include <engine/Texture.hpp>

class Material
{
public:
    Material(const char *vertexPath, const char *fragmentPath) : shader(Shader::FromFiles(vertexPath, fragmentPath)) {};
    Material() : shader(Shader::Default()) {};
    ~Material() = default;

    Texture &addTexture(const Texture &texture, std::string name = "")
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

    Shader &setShader(const Shader &shaderHandle)
    {
        shader = shaderHandle;
        return shader;
    }

    const Shader &getShader() const
    {
        return shader;
    }

    const std::vector<Texture> &getTextures() const
    {
        return textures;
    }

    const std::vector<std::string> &getTextureNames() const
    {
        return textureNames;
    }

private:
    std::vector<Texture> textures;
    std::vector<std::string> textureNames;
    Shader shader;
};