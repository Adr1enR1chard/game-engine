#pragma once

#include <service/resource/MaterialResource.hpp>
#include <service/resource/ShaderResource.hpp>

struct PBRMaterialOptions {
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

class MaterialFactory : public Service
{
  public:
    MaterialFactory(MaterialResource& materialResource, ShaderResource& shaderResource)
        : m_materialResource(materialResource), m_shaderResource(shaderResource)
    {
    }

    MaterialRef PBR(const PBRMaterialOptions& options);

  private:
    MaterialResource& m_materialResource;
    ShaderResource&   m_shaderResource;
};