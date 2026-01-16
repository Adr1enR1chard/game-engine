#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include <engine/utils/types.hpp>
#include <model/Mesh.hpp>
#include <model/Texture.hpp>

class Material
{
  public:
    static std::shared_ptr<Material> Default();

  public:
    Material(unsigned int id) : ID(id) {};
    ~Material();

    void use(std::unordered_map<uint32_t, UniformValue>* uniforms, std::unordered_map<std::string, Texture>* textures,
             glm::mat4 viewMatrix, glm::mat4 projectionMatrix) const;

    void setDirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity, float ambient) const;
    bool setBool(const std::string& name, bool value) const;
    bool setInt(const std::string& name, int value) const;
    bool setFloat(const std::string& name, float value) const;
    bool setMat4(const std::string& name, const glm::mat4 mat) const;
    bool setVec3(const std::string& name, const glm::vec3& value) const;

  private:
    static std::shared_ptr<Material> FromFiles(const char* vertexPath, const char* fragmentPath);
    static std::shared_ptr<Material> FromSource(const char* vertexSource, const char* fragmentSource);

    void checkCompileErrors(unsigned int shader, std::string type);

    static std::shared_ptr<Material> m_defaultMaterial;

  public:
    unsigned int ID;
};
