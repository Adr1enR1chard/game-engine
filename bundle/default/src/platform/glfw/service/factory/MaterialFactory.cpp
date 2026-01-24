#include <service/factory/MaterialFactory.hpp>

#include <engine/utils/Log.hpp>

MaterialRef MaterialFactory::PBR(const PBRMaterialOptions& options)
{
    // Get or create the PBR shader
    ShaderRef pbrShaderRef =
        m_shaderResource.create("PBRShader", "assets/shaders/Default.vert", "assets/shaders/PBR.frag");

    // Create a new material with the PBR shader
    MaterialRef materialRef = m_materialResource.create(pbrShaderRef);

    // Set the PBR uniforms
    m_materialResource.setUniform(materialRef, "material.baseColor", options.baseColor);
    m_materialResource.setUniform(materialRef, "material.metallic", options.metallic);
    m_materialResource.setUniform(materialRef, "material.roughness", options.roughness);
    m_materialResource.setUniform(materialRef, "material.ao", options.ao);
    m_materialResource.setUniform(materialRef, "material.useBaseColorMap", options.baseColorMap != nullptr);
    m_materialResource.setUniform(materialRef, "material.useNormalMap", options.normalMap != nullptr);
    m_materialResource.setUniform(materialRef, "material.useMetallicMap", options.metallicMap != nullptr);
    m_materialResource.setUniform(materialRef, "material.useRoughnessMap", options.roughnessMap != nullptr);
    m_materialResource.setUniform(materialRef, "material.useAOMap", options.aoMap != nullptr);

    return materialRef;
};