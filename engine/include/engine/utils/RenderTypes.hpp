#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <variant>

namespace engine
{

    using Index = unsigned int;

    using MeshRef = unsigned int;

    using ShaderRef = unsigned int;

    using TextureRef = unsigned int;

    using MaterialRef = unsigned int;

    using ModelRef = unsigned int;

    using FramebufferRef = unsigned int;

    enum TextureType
    {
        Texture2D,
        CubeMap,
        DepthMap
    };

    struct TextureUniform
    {
        TextureRef textureRef;
        TextureType type;
    };

    struct FramebufferUniform
    {
        FramebufferRef framebufferRef;
    };

    using UniformValue = std::variant<int, float, glm::vec2, glm::vec3, glm::vec4, glm::mat4, TextureUniform, FramebufferUniform>;

    using UniformCollection = std::unordered_map<std::string, UniformValue>;

} // namespace engine
