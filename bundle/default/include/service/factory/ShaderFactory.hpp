#pragma once

#include <engine/model/Service.hpp>
#include <service/factory/TextureFactory.hpp>
#include <engine/service/resource/ShaderResource.hpp>

namespace default_bundle
{

  class ShaderFactory : public engine::Service
  {
  public:
    ShaderFactory(engine::ShaderResource &shaderResource, TextureFactory &textureFactory)
        : m_shaderResource(shaderResource), m_textureFactory(textureFactory) {};
    ~ShaderFactory() override = default;

  public:
    engine::ShaderRef CustomShader(const char *name, const char *vertexShaderPath, const char *fragmentShaderPath,
                                   const engine::UniformCollection &defaultUniforms, const engine::ShaderParameters &params = {});
    engine::ShaderRef PBRShader(const char *name);
    engine::ShaderRef PhongShader(const char *name);
    engine::ShaderRef SkyboxShader(const char *name);

  private:
    void loadShaderFromFile(const char *filePath, std::string &outShaderCode);

    engine::ShaderResource &m_shaderResource;
    TextureFactory &m_textureFactory;
  };

} // namespace default_bundle