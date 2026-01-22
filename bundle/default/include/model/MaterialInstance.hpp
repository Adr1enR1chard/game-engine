#pragma once

#include <model/Texture.hpp>

#include <engine/utils/types.hpp>

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

class Material;

struct PBRMaterialParameters {
    glm::vec3 albedo       = glm::vec3(1.0f);
    float     metallic     = 1.0f;
    float     roughness    = 1.0f;
    float     ao           = 1.0f;
    Texture   albedoMap    = Texture::White();
    Texture   metallicMap  = Texture::White();
    Texture   roughnessMap = Texture::White();
    Texture   aoMap        = Texture::Black();
    Texture   normalMap    = Texture::DefaultNormalMap();
};

struct PhongMaterialParameters {
    glm::vec3 ambient     = glm::vec3(1.0f);
    glm::vec3 diffuse     = glm::vec3(1.0f);
    glm::vec3 specular    = glm::vec3(1.0f);
    float     shininess   = 32.0f;
    Texture   diffuseMap  = Texture::White();
    Texture   specularMap = Texture::White();
    Texture   normalMap   = Texture::DefaultNormalMap();
};

struct SkyboxMaterialParameters {
    glm::vec3 color    = glm::vec3(1.0f);
    Texture   colorMap = Texture::White();
};

class MaterialInstance
{
  public:
    MaterialInstance(std::shared_ptr<Material> baseMaterial) : m_material(baseMaterial) {}
    ~MaterialInstance() = default;

    static MaterialInstance PBR(const PBRMaterialParameters& params = {});
    static MaterialInstance Phong(const PhongMaterialParameters& params = {});
    static MaterialInstance Skybox(const SkyboxMaterialParameters& params = {});
    MaterialInstance&       setTexture(std::string name, const Texture& texture);
    MaterialInstance&       setUniform(const std::string& name, const UniformValue& value);

    void setCamera(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
    void setTransform(glm::mat4 modelMatrix);

  private:
    std::unordered_map<std::string, Texture>   m_textures;
    std::unordered_map<uint32_t, UniformValue> m_uniforms;
    std::shared_ptr<Material>                  m_material;
};