#include <service/factory/MaterialFactory.hpp>

#include <engine/utils/Log.hpp>

namespace default_bundle
{

    using namespace engine;

    Material MaterialFactory::PBRMaterial(const PBRMaterialParameters &options)
    {
        ShaderRef pbrShaderRef = m_shaderFactory.PBRShader("__PBRShader");

        UniformCollection uniforms = {
            {"material.baseColor", options.baseColor},
            {"material.metallic", options.metallic},
            {"material.roughness", options.roughness},
            {"material.ao", options.ao},
            {"material.useMetallicRoughnessMap", options.useMetallicRoughnessMap},
        };

        if (options.baseColorMap != 0)
            uniforms["material.baseColorMap"] = TextureUniform{options.baseColorMap, Texture2D};
        if (options.normalMap != 0)
            uniforms["material.normalMap"] = TextureUniform{options.normalMap, Texture2D};
        if (options.metallicMap != 0)
            uniforms["material.metallicMap"] = TextureUniform{options.metallicMap, Texture2D};
        if (options.roughnessMap != 0)
            uniforms["material.roughnessMap"] = TextureUniform{options.roughnessMap, Texture2D};
        if (options.aoMap != 0)
            uniforms["material.aoMap"] = TextureUniform{options.aoMap, Texture2D};
        return CustomMaterial(pbrShaderRef, uniforms);
    };

    Material MaterialFactory::PhongMaterial(const PhongMaterialParameters &options)
    {
        ShaderRef phongShaderRef = m_shaderFactory.PhongShader("__PhongShader");

        UniformCollection uniforms = {
            {"material.ambient", options.ambient},
            {"material.diffuse", options.diffuse},
            {"material.specular", options.specular},
            {"material.shininess", options.shininess},
        };

        if (options.diffuseMap != 0)
            uniforms["material.diffuseMap"] = TextureUniform{options.diffuseMap, Texture2D};
        if (options.specularMap != 0)
            uniforms["material.specularMap"] = TextureUniform{options.specularMap, Texture2D};
        if (options.normalMap != 0)
            uniforms["material.normalMap"] = TextureUniform{options.normalMap, Texture2D};
        return CustomMaterial(phongShaderRef, uniforms);
    }

    Material MaterialFactory::SkyboxMaterial(const SkyboxMaterialParameters &options)
    {
        ShaderRef skyboxShaderRef = m_shaderFactory.SkyboxShader("__SkyboxShader");

        UniformCollection uniforms;
        if (options.colorMap != 0)
            uniforms["material.colorMap"] = TextureUniform{options.colorMap, CubeMap};

        return CustomMaterial(skyboxShaderRef, uniforms);
    }

    Material MaterialFactory::CustomMaterial(ShaderRef shaderRef, const UniformCollection &uniforms)
    {
        return {shaderRef, uniforms};
    }
} // namespace default_bundle
