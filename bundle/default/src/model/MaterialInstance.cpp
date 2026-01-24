#include <model/MaterialInstance.hpp>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <model/Material.hpp>

MaterialInstance MaterialInstance::PBR(const PBRMaterialParameters_& params)
{
    MaterialInstance material = MaterialInstance(Material::PBR());

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

MaterialInstance MaterialInstance::Phong(const PhongMaterialParameters& params)
{
    MaterialInstance material = MaterialInstance(Material::Phong());

    material.setUniform("material.ambient", params.ambient);
    material.setUniform("material.diffuse", params.diffuse);
    material.setUniform("material.specular", params.specular);
    material.setUniform("material.shininess", params.shininess);
    material.setTexture("material.diffuseMap", params.diffuseMap);
    material.setTexture("material.specularMap", params.specularMap);
    material.setTexture("material.normalMap", params.normalMap);

    return material;
}

MaterialInstance MaterialInstance::Skybox(const SkyboxMaterialParameters_& params)
{
    MaterialInstance material = MaterialInstance(Material::Skybox());

    material.setUniform("material.color", params.color);
    material.setTexture("material.colorMap", params.colorMap);

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

            if constexpr (std::is_same_v<T, Uniform::DirectionalLight>) {
                setUniform(name + ".direction", v.direction);
                setUniform(name + ".color", v.color);
                setUniform(name + ".intensity", v.intensity);
            } else if constexpr (std::is_same_v<T, Uniform::PointLight>) {
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

void MaterialInstance::setCamera(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    m_material->use(&m_uniforms, &m_textures, viewMatrix, projectionMatrix);
}

void MaterialInstance::setTransform(glm::mat4 modelMatrix)
{
    m_material->setMat4("model", modelMatrix);
}
