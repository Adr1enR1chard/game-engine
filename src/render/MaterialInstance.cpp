#include "engine/MaterialInstance.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <engine/Material.hpp>

MaterialInstance MaterialInstance::Default(const DefaultMaterialParameters& params)
{
    MaterialInstance material = MaterialInstance(Material::Default());

    material.setUniform("material.albedo", params.albedo);
    material.setUniform("material.metallic", params.metallic);
    material.setUniform("material.roughness", params.roughness);
    material.setUniform("material.ao", params.ao);
    material.setTexture("material.albedoMap", params.albedoMap);
    material.setTexture("material.metallicMap", params.metallicMap);
    material.setTexture("material.roughnessMap", params.roughnessMap);
    material.setTexture("material.aoMap", params.aoMap);
    material.setTexture("material.normalMap", params.normalMap);

    return material;
}

MaterialInstance& MaterialInstance::setTexture(std::string name, const Texture& texture)
{
    if (m_textures.size() >= 32) {
        throw std::runtime_error("MaterialHandle can only hold up to 32 textures.");
    }

    m_textures[name] = texture;
    return *this;
}

MaterialInstance& MaterialInstance::setUniform(const std::string& name, const UniformValue& value)
{
    std::visit(
        [&](auto const& v) {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, UDirectionalLight>) {
                setUniform(name + ".direction", v.direction);
                setUniform(name + ".color", v.color);
                setUniform(name + ".intensity", v.intensity);
            } else if constexpr (std::is_same_v<T, UPointLight>) {
                setUniform(name + ".position", v.position);
                setUniform(name + ".color", v.color);
                setUniform(name + ".intensity", v.intensity);
            } else {
                uint32_t location = glGetUniformLocation(m_material->ID, name.c_str());
                if (location != static_cast<uint32_t>(-1)) {
                    m_uniforms[location] = value;
                }
            }
        },
        value);

    return *this;
}

// void MaterialInstance::applyUniforms()
// {
//     m_material->use();

//     for (const auto& [location, value] : m_uniforms) {
//         std::visit(
//             [&](auto&& arg) {
//                 using T = std::decay_t<decltype(arg)>;
//                 if constexpr (std::is_same_v<T, int>) {
//                     glUniform1i(location, arg);
//                 } else if constexpr (std::is_same_v<T, float>) {
//                     glUniform1f(location, arg);
//                 } else if constexpr (std::is_same_v<T, glm::vec2>) {
//                     glUniform2fv(location, 1, glm::value_ptr(arg));
//                 } else if constexpr (std::is_same_v<T, glm::vec3>) {
//                     glUniform3fv(location, 1, glm::value_ptr(arg));
//                 } else if constexpr (std::is_same_v<T, glm::vec4>) {
//                     glUniform4fv(location, 1, glm::value_ptr(arg));
//                 } else if constexpr (std::is_same_v<T, glm::mat4>) {
//                     glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(arg));
//                 }
//             },
//             value);
//     }
// }

void MaterialInstance::setup(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    m_material->use(&m_uniforms, &m_textures, viewMatrix, projectionMatrix);
}

void MaterialInstance::link(glm::mat4 modelMatrix)
{
    m_material->setMat4("model", modelMatrix);
}

// std::shared_ptr<Material> MaterialInstance::getMaterial()
// {
//     return m_material;
// }

// void MaterialInstance::bindTextures()
// {
//     m_material->use();

//     if (m_textures.empty()) {
//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_2D, 0);
//     }

//     unsigned int i = 0;
//     for (const auto& [name, texture] : m_textures) {
//         glActiveTexture(GL_TEXTURE0 + i);
//         glBindTexture(GL_TEXTURE_2D, texture.ID);

//         GLint location = glGetUniformLocation(m_material->ID, name.c_str());
//         if (location >= 0)
//             glUniform1i(location, i);
//         ++i;
//     }
// }