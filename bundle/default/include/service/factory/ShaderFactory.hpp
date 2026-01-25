#pragma once

#include <engine/model/Service.hpp>
#include <service/factory/TextureFactory.hpp>
#include <service/resource/ShaderResource.hpp>

class ShaderFactory : public Service
{
  public:
    ShaderFactory(ShaderResource& shaderResource, TextureFactory& textureFactory)
        : m_shaderResource(shaderResource), m_textureFactory(textureFactory) {};
    ~ShaderFactory() override = default;

  public:
    ShaderRef PBR(const char* name);
    ShaderRef Phong(const char* name);
    ShaderRef Skybox(const char* name);

  private:
    ShaderResource& m_shaderResource;
    TextureFactory& m_textureFactory;
};