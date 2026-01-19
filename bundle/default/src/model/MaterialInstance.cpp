#include <model/MaterialInstance.hpp>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <model/Material.hpp>

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

void MaterialInstance::setup(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    m_material->use(&m_uniforms, &m_textures, viewMatrix, projectionMatrix);
}

void MaterialInstance::link(glm::mat4 modelMatrix)
{
    m_material->setMat4("model", modelMatrix);
}
