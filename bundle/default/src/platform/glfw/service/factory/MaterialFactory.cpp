#include <service/factory/MaterialFactory.hpp>

#include <engine/utils/Log.hpp>

MaterialRef MaterialFactory::PBR(const PBRMaterialParameters& options)
{
    ShaderRef pbrShaderRef =
        m_shaderResource.create("PBRShader", "assets/shaders/Default.vert", "assets/shaders/PBR.frag");

    MaterialRef materialRef = m_materialResource.create(pbrShaderRef);

    m_materialResource.setUniform(materialRef, "material.baseColor", options.baseColor);
    m_materialResource.setUniform(materialRef, "material.metallic", options.metallic);
    m_materialResource.setUniform(materialRef, "material.roughness", options.roughness);
    m_materialResource.setUniform(materialRef, "material.ao", options.ao);

    m_materialResource.setUniform(materialRef, "material.useBaseColorMap", options.baseColorMap != 0);
    if (options.baseColorMap != 0) {
        m_materialResource.setUniform(materialRef, "material.baseColorMap", options.baseColorMap);
    }

    m_materialResource.setUniform(materialRef, "material.useNormalMap", options.normalMap != 0);
    if (options.normalMap != 0) {
        m_materialResource.setUniform(materialRef, "material.normalMap", options.normalMap);
    }

    m_materialResource.setUniform(materialRef, "material.useMetallicMap", options.metallicMap != 0);
    if (options.metallicMap != 0) {
        m_materialResource.setUniform(materialRef, "material.metallicMap", options.metallicMap);
    }

    m_materialResource.setUniform(materialRef, "material.useRoughnessMap", options.roughnessMap != 0);
    if (options.roughnessMap != 0) {
        m_materialResource.setUniform(materialRef, "material.roughnessMap", options.roughnessMap);
    }

    m_materialResource.setUniform(materialRef, "material.useAOMap", options.aoMap != 0);
    if (options.aoMap != 0) {
        m_materialResource.setUniform(materialRef, "material.aoMap", options.aoMap);
    }

    return materialRef;
};

MaterialRef MaterialFactory::Skybox(const SkyboxMaterialParameters& options)
{
    ShaderRef skyboxShaderRef =
        m_shaderResource.create("SkyboxShader", "assets/shaders/Skybox.vert", "assets/shaders/Skybox.frag",
                                {
                                    .cullFaceEnabled   = false,
                                    .depthTestEnabled  = false,
                                    .depthWriteEnabled = false,
                                });
    MaterialRef materialRef = m_materialResource.create(skyboxShaderRef);

    m_materialResource.setUniform(materialRef, "material.colorMap", options.colorMap);

    return materialRef;
}