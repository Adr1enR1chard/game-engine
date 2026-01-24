#pragma once

#include <service/factory/TextureFactory.hpp>
#include <service/resource/MaterialResource.hpp>
#include <service/resource/ShaderResource.hpp>

struct PBRMaterialParameters {
    glm::vec3  baseColor    = glm::vec3(1.0f);
    float      metallic     = 0.0f;
    float      roughness    = 1.0f;
    float      ao           = 1.0f;
    TextureRef baseColorMap = 0;
    TextureRef normalMap    = 0;
    TextureRef metallicMap  = 0;
    TextureRef roughnessMap = 0;
    TextureRef aoMap        = 0;
};

struct SkyboxMaterialParameters {
    TextureRef colorMap = 0;
};

class MaterialFactory : public Service
{
  public:
    MaterialFactory(MaterialResource& materialResource, ShaderResource& shaderResource, TextureFactory& textureFactory)
        : m_materialResource(materialResource), m_shaderResource(shaderResource), m_textureFactory(textureFactory)
    {
    }

    MaterialRef PBR(const PBRMaterialParameters& options);
    MaterialRef Skybox(const SkyboxMaterialParameters& options);

  private:
    MaterialResource& m_materialResource;
    ShaderResource&   m_shaderResource;
    TextureFactory&   m_textureFactory;
};