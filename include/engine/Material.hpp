#pragma once
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <variant>

#include <engine/Shader.hpp>
#include <engine/Texture.hpp>

using UniformValue = std::variant<
    int,
    float,
    glm::vec2,
    glm::vec3,
    glm::vec4,
    glm::mat4>;

struct UniformWrapper
{
    uint32_t location;
    UniformValue value;
};

class Material
{
public:
    Material(Shader shader) : shaderProgram(shader) {};
    Material() : shaderProgram(Shader::Default()) {};
    ~Material() = default;
    static std::shared_ptr<Material> Default();

    Texture &setTexture(std::string name, const Texture &texture);
    void setUniform(const std::string &name, const UniformValue &value);

private:
    void applyUniforms();
    void bindTextures() const;

    Shader &setShader(const Shader &shaderHandle);
    Shader &getShader();

    friend class RenderSystem;
    friend class LightSystem;

private:
    std::unordered_map<std::string, Texture> textureMap;
    Shader shaderProgram;
    std::unordered_map<uint32_t, UniformValue> uniforms;
};