#include "engine/Material.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

std::shared_ptr<Material> Material::Default()
{
    std::shared_ptr<Material> material = std::make_shared<Material>(Shader::Default());

    // Set default uniforms
    material->setUniform("albedo", glm::vec3(1.0f, 1.0f, 1.0f));
    material->setUniform("shininess", 32.0f);
    material->setTexture("albedoMap", Texture::White());

    return material;
}

Texture &Material::setTexture(std::string name, const Texture &texture)
{
    if (textureMap.size() >= 32)
    {
        throw std::runtime_error("MaterialHandle can only hold up to 32 textures.");
    }

    textureMap[name] = texture;

    return textureMap[name];
}

void Material::setUniform(const std::string &name, const UniformValue &value)
{
    uint32_t location = glGetUniformLocation(shaderProgram.ID, name.c_str());
    if (location == static_cast<uint32_t>(-1))
    {
        return;
    }

    uniforms[location] = value;
}

void Material::applyUniforms()
{
    shaderProgram.use();

    for (const auto &[location, value] : uniforms)
    {
        std::visit([&](auto &&arg)
                   {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>)
            {
                glUniform1i(location, arg);
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                glUniform1f(location, arg);
            }
            else if constexpr (std::is_same_v<T, glm::vec2>)
            {
                glUniform2fv(location, 1, glm::value_ptr(arg));
            }
            else if constexpr (std::is_same_v<T, glm::vec3>)
            {
                glUniform3fv(location, 1, glm::value_ptr(arg));
            }
            else if constexpr (std::is_same_v<T, glm::vec4>)
            {
                glUniform4fv(location, 1, glm::value_ptr(arg));
            }
            else if constexpr (std::is_same_v<T, glm::mat4>)
            {
                glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(arg));
            } },
                   value);
    }
}

void Material::bindTextures() const
{
    shaderProgram.use();

    if (textureMap.empty())
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    unsigned int i = 0;
    for (const auto &[name, texture] : textureMap)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture.ID);

        GLint location = glGetUniformLocation(shaderProgram.ID, name.c_str());
        if (location >= 0)
            glUniform1i(location, i);
        ++i;
    }
}

Shader &Material::setShader(const Shader &shaderHandle)
{
    shaderProgram = shaderHandle;
    return shaderProgram;
}

Shader &Material::getShader()
{
    return shaderProgram;
}