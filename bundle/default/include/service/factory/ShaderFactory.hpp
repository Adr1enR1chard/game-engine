#pragma once

#include <engine/model/Service.hpp>
#include <service/factory/TextureFactory.hpp>
#include <service/resource/ShaderResource.hpp>

class ShaderFactory : public Service
{
public:
  ShaderFactory(ShaderResource &shaderResource, TextureFactory &textureFactory)
      : m_shaderResource(shaderResource), m_textureFactory(textureFactory) {};
  ~ShaderFactory() override = default;

public:
  ShaderRef CustomShader(const char *name, const char *vertexShaderPath, const char *fragmentShaderPath,
                         const UniformCollection &defaultUniforms, const ShaderParameters &params = {});
  ShaderRef PBRShader(const char *name);
  ShaderRef PhongShader(const char *name);
  ShaderRef SkyboxShader(const char *name);

private:
  void loadShaderFromFile(const char *filePath, std::string &outShaderCode);

  ShaderResource &m_shaderResource;
  TextureFactory &m_textureFactory;
};