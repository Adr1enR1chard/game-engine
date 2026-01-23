#pragma once

#include <engine/model/Service.hpp>
#include <utils/RenderTypes.hpp>

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <utils/IdManager.hpp>

class ShaderResource : public Service
{
  public:
    ShaderResource()           = default;
    ~ShaderResource() override = default;

  public:
    /// @brief Creates a new shader resource, if a shader with the same name
    /// already exists, it returns the existing one.
    /// @param name
    /// @param vertexShaderPath
    /// @param fragmentShaderPath
    /// @return
    ShaderRef create(const char* name, const char* vertexShaderPath, const char* fragmentShaderPath);
    ShaderRef get(const char* name) const;

    void bind(ShaderRef shaderRef, const UniformCollection* uniforms, glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
              glm::mat4 modelMatrix, class TextureResource& textureResource) const;

  private:
    struct ShaderData;

    struct ShaderDataDeleter {
        void operator()(ShaderData* shaderData);
    };

    IdManager                                                                     m_idManager;
    std::unordered_map<std::string, ShaderRef>                                    m_nameToShaderRef;
    std::unordered_map<ShaderRef, std::unique_ptr<ShaderData, ShaderDataDeleter>> m_loadedShaders;
};