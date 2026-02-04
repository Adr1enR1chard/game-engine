#pragma once

#include <engine/model/Service.hpp>
#include <engine/bundle/standalone/service/Renderer.hpp>

#include <service/factory/TextureFactory.hpp>

namespace default_bundle
{
  using namespace engine;
  class ShaderFactory : public Service
  {
  public:
    ShaderFactory(Renderer &renderer, TextureFactory &textureFactory)
        : m_renderer(renderer), m_textureFactory(textureFactory) {};
    ~ShaderFactory() override = default;

  public:
    ShaderRef CustomShader(const char *name, const char *vertexShaderPath, const char *fragmentShaderPath,
                           const UniformCollection &defaultUniforms = {}, const Renderer::ShaderParameters &params = {});
    ShaderRef PBRShader(const char *name);
    ShaderRef PhongShader(const char *name);
    ShaderRef SkyboxShader(const char *name);

  private:
    void loadShaderFromFile(const char *filePath, std::string &outShaderCode);

    Renderer &m_renderer;
    TextureFactory &m_textureFactory;
  };

} // namespace default_bundle