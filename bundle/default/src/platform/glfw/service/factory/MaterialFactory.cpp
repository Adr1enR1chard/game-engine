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

    m_materialResource.setUniform(materialRef, "material.useBaseColorMap", options.baseColorMap != 0);
    if (options.baseColorMap != 0) {
        m_materialResource.setUniform(materialRef, "material.baseColorMap",
                                      Uniform::Texture{options.baseColorMap, TextureType::Texture2D});
    }

    m_materialResource.setUniform(materialRef, "material.useNormalMap", options.normalMap != 0);
    if (options.normalMap != 0) {
        m_materialResource.setUniform(materialRef, "material.normalMap",
                                      Uniform::Texture{options.normalMap, TextureType::Texture2D});
    }

    m_materialResource.setUniform(materialRef, "material.useMetallicMap", options.metallicMap != 0);
    if (options.metallicMap != 0) {
        m_materialResource.setUniform(materialRef, "material.metallicMap",
                                      Uniform::Texture{options.metallicMap, TextureType::Texture2D});
    }

    m_materialResource.setUniform(materialRef, "material.useRoughnessMap", options.roughnessMap != 0);
    if (options.roughnessMap != 0) {
        m_materialResource.setUniform(materialRef, "material.roughnessMap",
                                      Uniform::Texture{options.roughnessMap, TextureType::Texture2D});
    }

    m_materialResource.setUniform(materialRef, "material.useAOMap", options.aoMap != 0);
    if (options.aoMap != 0) {
        m_materialResource.setUniform(materialRef, "material.aoMap",
                                      Uniform::Texture{options.aoMap, TextureType::Texture2D});
    }

    return materialRef;
};