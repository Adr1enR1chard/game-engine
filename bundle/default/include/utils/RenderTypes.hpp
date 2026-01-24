#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <variant>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

using Index = unsigned int;

using MeshRef = unsigned int;

using ShaderRef = unsigned int;

using TextureRef = unsigned int;

using MaterialRef = unsigned int;

class Uniform
{
  public:
    struct DirectionalLight {
        glm::vec3 direction;
        glm::vec3 color     = glm::vec3(1.0f);
        float     ambient   = 0.1f;
        float     intensity = 1.0f;
    };

    struct PointLight {
        glm::vec3 position;
        glm::vec3 color     = glm::vec3(1.0f);
        float     intensity = 1.0f;
    };

    struct Texture {
        TextureRef  textureRef;
        const char* textureName;
    };
};

using UniformValue = std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4, Uniform::DirectionalLight,
                                  Uniform::PointLight, Uniform::Texture>;

using UniformCollection = std::unordered_map<std::string, UniformValue>;
