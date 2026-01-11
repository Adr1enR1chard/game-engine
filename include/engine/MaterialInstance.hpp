#pragma once

#include "utils/types.hpp"
#include <engine/Texture.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

class Material;

struct DefaultMaterialParameters {
    glm::vec3 albedo       = glm::vec3(1.0f);
    float     metallic     = 1.0f;
    float     roughness    = 1.0f;
    float     ao           = 1.0f;
    Texture   albedoMap    = Texture::White();
    Texture   metallicMap  = Texture::White();
    Texture   roughnessMap = Texture::White();
    Texture   aoMap        = Texture::White();
    Texture   normalMap    = Texture::DefaultNormalMap();
};

class MaterialInstance
{
  public:
    MaterialInstance(std::shared_ptr<Material> baseMaterial) : m_material(baseMaterial) {}
    ~MaterialInstance() = default;

    static MaterialInstance Default(const DefaultMaterialParameters& params = {});

    MaterialInstance& setTexture(std::string name, const Texture& texture);
    MaterialInstance& setUniform(const std::string& name, const UniformValue& value);

  private:
    friend class RenderSystem;

    void setup(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

  private:
    friend class Model;
    friend class Mesh;
    void link(glm::mat4 modelMatrix);

  private:
    std::unordered_map<std::string, Texture>   m_textures;
    std::unordered_map<uint32_t, UniformValue> m_uniforms;
    std::shared_ptr<Material>                  m_material;
};