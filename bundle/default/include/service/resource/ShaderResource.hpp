#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include <engine/model/Service.hpp>
#include <service/resource/TextureResource.hpp>
#include <utils/IdManager.hpp>
#include <utils/RenderTypes.hpp>

struct ShaderParameters {
    bool cullFaceEnabled   = true;
    bool depthTestEnabled  = true;
    bool depthWriteEnabled = true;
    bool backfaceCulling   = true;
    bool blendEnabled      = false;
    bool wireframeEnabled  = false;
    bool scissorEnabled    = false;
    bool stencilEnabled    = false;
};

class ShaderResource : public Service
{
  public:
    ShaderResource(TextureResource& textureResource) : m_textureResource(textureResource) {};
    ~ShaderResource() override = default;

  public:
    ShaderRef create(const char* name, const char* vertexShaderPath, const char* fragmentShaderPath,
                     const ShaderParameters& params = {});
    ShaderRef get(const char* name) const;

    void bind(ShaderRef shaderRef, const UniformCollection* uniforms, glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
              glm::mat4 modelMatrix) const;

  private:
    /// @brief Implementation details for Shader, based on the rendering backend
    struct ShaderImpl;
    struct ShaderImplDeleter {
        void operator()(ShaderImpl* shaderImpl);
    };

    struct ShaderData {
        std::unique_ptr<ShaderImpl, ShaderImplDeleter> impl;
        ShaderParameters                               parameters = {};
    };

    IdManager                                                  m_idManager;
    std::unordered_map<std::string, ShaderRef>                 m_nameToShaderRef;
    std::unordered_map<ShaderRef, std::unique_ptr<ShaderData>> m_loadedShaders;
    TextureResource&                                           m_textureResource;
};