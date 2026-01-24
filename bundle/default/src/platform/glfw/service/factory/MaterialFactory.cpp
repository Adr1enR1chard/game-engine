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

    m_materialResource.setUniform(materialRef, "material.baseColorMap",
                                  options.baseColorMap != 0 ? options.baseColorMap : m_textureFactory.WhiteTexture2D());

    m_materialResource.setUniform(materialRef, "material.normalMap",
                                  options.normalMap != 0 ? options.normalMap : m_textureFactory.NormalTexture2D());

    m_materialResource.setUniform(materialRef, "material.metallicMap",
                                  options.metallicMap != 0 ? options.metallicMap : m_textureFactory.BlackTexture2D());

    m_materialResource.setUniform(materialRef, "material.roughnessMap",
                                  options.roughnessMap != 0 ? options.roughnessMap : m_textureFactory.WhiteTexture2D());

    m_materialResource.setUniform(materialRef, "material.aoMap",
                                  options.aoMap != 0 ? options.aoMap : m_textureFactory.WhiteTexture2D());

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