#pragma once

#include <service/resource/MaterialResource.hpp>
#include <service/resource/ShaderResource.hpp>

struct PBRMaterialOptions {
    glm::vec3   baseColor    = glm::vec3(1.0f);
    float       metallic     = 0.0f;
    float       roughness    = 1.0f;
    float       ao           = 1.0f;
    const char* baseColorMap = nullptr;
    const char* normalMap    = nullptr;
    const char* metallicMap  = nullptr;
    const char* roughnessMap = nullptr;
    const char* aoMap        = nullptr;
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