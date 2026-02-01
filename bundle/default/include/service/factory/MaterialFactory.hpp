#pragma once

#include <service/factory/ShaderFactory.hpp>
#include <service/factory/TextureFactory.hpp>
#include <engine/service/resource/MaterialResource.hpp>

namespace default_bundle
{

    struct PBRMaterialParameters
    {
        glm::vec3 baseColor = glm::vec3(1.0f);
        float metallic = 1.0f;
        float roughness = 1.0f;
        float ao = 1.0f;
        bool useMetallicRoughnessMap = false;
        engine::TextureRef baseColorMap = 0;
        engine::TextureRef normalMap = 0;
        engine::TextureRef metallicMap = 0;
        engine::TextureRef roughnessMap = 0;
        engine::TextureRef aoMap = 0;
    };

    struct PhongMaterialParameters
    {
        glm::vec3 ambient = glm::vec3(0.1f);
        glm::vec3 diffuse = glm::vec3(1.0f);
        glm::vec3 specular = glm::vec3(1.0f);
        float shininess = 32.0f;
        engine::TextureRef diffuseMap = 0;
        engine::TextureRef specularMap = 0;
        engine::TextureRef normalMap = 0;
    };

    struct SkyboxMaterialParameters
    {
        engine::TextureRef colorMap = 0;
    };

    class MaterialFactory : public engine::Service
    {
    public:
        MaterialFactory(engine::MaterialResource &materialResource, ShaderFactory &shaderFactory)
            : m_materialResource(materialResource), m_shaderFactory(shaderFactory)
        {
        }

        engine::MaterialRef PBRMaterial(const PBRMaterialParameters &options = {});
        engine::MaterialRef PhongMaterial(const PhongMaterialParameters &options = {});
        engine::MaterialRef SkyboxMaterial(const SkyboxMaterialParameters &options = {});
        engine::MaterialRef CustomMaterial(engine::ShaderRef shaderRef, const engine::UniformCollection &defaultUniforms = {});

    private:
        engine::MaterialResource &m_materialResource;
        ShaderFactory &m_shaderFactory;
    };

} // namespace default_bundle