#include <engine/bundle/core/service/factory/MaterialFactory.hpp>

#include <engine/utils/Log.hpp>

namespace engine
{

    MaterialHandle MaterialFactory::PBRMaterial(const PBRMaterialParameters &options)
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
            uniforms["material.baseColorMap"] = Sampler2D{options.baseColorMap};
        if (options.normalMap != 0)
            uniforms["material.normalMap"] = Sampler2D{options.normalMap};
        if (options.metallicMap != 0)
            uniforms["material.metallicMap"] = Sampler2D{options.metallicMap};
        if (options.roughnessMap != 0)
            uniforms["material.roughnessMap"] = Sampler2D{options.roughnessMap};
        if (options.aoMap != 0)
            uniforms["material.aoMap"] = Sampler2D{options.aoMap};
        return CustomMaterial(pbrShaderRef, uniforms);
    };

    MaterialHandle MaterialFactory::PhongMaterial(const PhongMaterialParameters &options)
    {
        ShaderRef phongShaderRef = m_shaderFactory.PhongShader("__PhongShader");

        UniformCollection uniforms = {
            {"material.ambient", options.ambient},
            {"material.diffuse", options.diffuse},
            {"material.specular", options.specular},
            {"material.shininess", options.shininess},
        };

        if (options.diffuseMap != 0)
            uniforms["material.diffuseMap"] = Sampler2D{options.diffuseMap};
        if (options.specularMap != 0)
            uniforms["material.specularMap"] = Sampler2D{options.specularMap};
        if (options.normalMap != 0)
            uniforms["material.normalMap"] = Sampler2D{options.normalMap};
        return CustomMaterial(phongShaderRef, uniforms);
    }

    MaterialHandle MaterialFactory::SkyboxMaterial(const SkyboxMaterialParameters &options)
    {
        ShaderRef skyboxShaderRef = m_shaderFactory.SkyboxShader("__SkyboxShader");

        UniformCollection uniforms;
        if (options.colorMap != 0)
            uniforms["material.colorMap"] = SamplerCube{options.colorMap};

        return CustomMaterial(skyboxShaderRef, uniforms);
    }

    MaterialHandle MaterialFactory::CustomMaterial(ShaderRef shaderRef, const UniformCollection &uniforms)
    {
        return {shaderRef, uniforms};
    }
} // namespace engine
