#pragma once

#include <service/factory/ShaderFactory.hpp>
#include <service/factory/TextureFactory.hpp>
#include <engine/service/resource/MaterialResource.hpp>

namespace default_bundle
{
    using namespace engine;

    struct Material
    {
        ShaderRef shaderRef;
        UniformCollection uniforms;

        bool isValid() const
        {
            return shaderRef != 0;
        }
    };

    class MaterialFactory : public Service
    {
    public:
        MaterialFactory(Renderer &renderer, ShaderFactory &shaderFactory)
            : m_renderer(renderer), m_shaderFactory(shaderFactory)
        {
        }

        struct PBRMaterialParameters
        {
            glm::vec3 baseColor = glm::vec3(1.0f);
            float metallic = 1.0f;
            float roughness = 1.0f;
            float ao = 1.0f;
            bool useMetallicRoughnessMap = false;
            TextureRef baseColorMap = 0;
            TextureRef normalMap = 0;
            TextureRef metallicMap = 0;
            TextureRef roughnessMap = 0;
            TextureRef aoMap = 0;
        };

        struct PhongMaterialParameters
        {
            glm::vec3 ambient = glm::vec3(0.1f);
            glm::vec3 diffuse = glm::vec3(1.0f);
            glm::vec3 specular = glm::vec3(1.0f);
            float shininess = 32.0f;
            TextureRef diffuseMap = 0;
            TextureRef specularMap = 0;
            TextureRef normalMap = 0;
        };

        struct SkyboxMaterialParameters
        {
            TextureRef colorMap = 0;
        };

        Material PBRMaterial(const PBRMaterialParameters &options = {});
        Material PhongMaterial(const PhongMaterialParameters &options = {});
        Material SkyboxMaterial(const SkyboxMaterialParameters &options = {});
        Material CustomMaterial(ShaderRef shaderRef, const UniformCollection &defaultUniforms = {});

    private:
        Renderer &m_renderer;
        ShaderFactory &m_shaderFactory;
    };

} // namespace default_bundle