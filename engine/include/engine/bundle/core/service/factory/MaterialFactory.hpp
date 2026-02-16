#pragma once

#include <engine/bundle/core/service/factory/ShaderFactory.hpp>
#include <engine/bundle/core/service/factory/TextureFactory.hpp>

namespace engine
{

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

    struct MaterialHandle
    {
        ShaderRef shaderRef;
        UniformCollection uniforms;

        bool isValid() const
        {
            return shaderRef != 0;
        }

        operator bool() const
        {
            return isValid();
        }

        bool *getBool(const std::string &name)
        {
            auto it = uniforms.find(name);
            if (it != uniforms.end())
            {
                return std::get_if<bool>(&(it->second));
            }
            return nullptr;
        }

        int *getInt(const std::string &name)
        {
            auto it = uniforms.find(name);
            if (it != uniforms.end())
            {
                return std::get_if<int>(&(it->second));
            }
            return nullptr;
        }

        glm::vec3 *getVec3(const std::string &name)
        {
            auto it = uniforms.find(name);
            if (it != uniforms.end())
            {
                return std::get_if<glm::vec3>(&(it->second));
            }
            return nullptr;
        }

        glm::mat4 *getMat4(const std::string &name)
        {
            auto it = uniforms.find(name);
            if (it != uniforms.end())
            {
                return std::get_if<glm::mat4>(&(it->second));
            }
            return nullptr;
        }

        glm::vec2 *getVec2(const std::string &name)
        {
            auto it = uniforms.find(name);
            if (it != uniforms.end())
            {
                return std::get_if<glm::vec2>(&(it->second));
            }
            return nullptr;
        }

        glm::vec4 *getVec4(const std::string &name)
        {
            auto it = uniforms.find(name);
            if (it != uniforms.end())
            {
                return std::get_if<glm::vec4>(&(it->second));
            }
            return nullptr;
        }

        float *getFloat(const std::string &name)
        {
            auto it = uniforms.find(name);
            if (it != uniforms.end())
            {
                return std::get_if<float>(&(it->second));
            }
            return nullptr;
        }

        Sampler2D *getSampler2D(const std::string &name)
        {
            auto it = uniforms.find(name);
            if (it != uniforms.end())
            {
                return std::get_if<Sampler2D>(&(it->second));
            }
            return nullptr;
        }

        SamplerCube *getSamplerCube(const std::string &name)
        {
            auto it = uniforms.find(name);
            if (it != uniforms.end())
            {
                return std::get_if<SamplerCube>(&(it->second));
            }
            return nullptr;
        }
    };

    class MaterialFactory : public Service
    {
    public:
        MaterialFactory(Renderer &renderer, ShaderFactory &shaderFactory)
            : m_renderer(renderer), m_shaderFactory(shaderFactory)
        {
        }

        MaterialHandle PBRMaterial(const PBRMaterialParameters &options = PBRMaterialParameters{});
        MaterialHandle PhongMaterial(const PhongMaterialParameters &options = PhongMaterialParameters{});
        MaterialHandle SkyboxMaterial(const SkyboxMaterialParameters &options = SkyboxMaterialParameters{});
        MaterialHandle CustomMaterial(ShaderRef shaderRef, const UniformCollection &defaultUniforms = UniformCollection{});

    private:
        Renderer &m_renderer;
        ShaderFactory &m_shaderFactory;
    };

} // namespace engine