#include <service/factory/MaterialFactory.hpp>

#include <engine/utils/Log.hpp>

MaterialRef MaterialFactory::PBR(const PBRMaterialParameters& options)
{
    ShaderRef pbrShaderRef = m_shaderFactory.PBR("__PBRShader");

    MaterialRef materialRef = m_materialResource.create(pbrShaderRef);

    m_materialResource.setUniform(materialRef, "material.baseColor", options.baseColor);
    m_materialResource.setUniform(materialRef, "material.metallic", options.metallic);
    m_materialResource.setUniform(materialRef, "material.roughness", options.roughness);
    m_materialResource.setUniform(materialRef, "material.ao", options.ao);

    if (options.baseColorMap != 0)
        m_materialResource.setUniform(materialRef, "material.baseColorMap", options.baseColorMap);
    if (options.normalMap != 0)
        m_materialResource.setUniform(materialRef, "material.normalMap", options.normalMap);
    if (options.metallicMap != 0)
        m_materialResource.setUniform(materialRef, "material.metallicMap", options.metallicMap);
    if (options.roughnessMap != 0)
        m_materialResource.setUniform(materialRef, "material.roughnessMap", options.roughnessMap);
    if (options.aoMap != 0)
        m_materialResource.setUniform(materialRef, "material.aoMap", options.aoMap);

    return materialRef;
};

MaterialRef MaterialFactory::Phong(const PhongMaterialParameters& options)
{
    ShaderRef phongShaderRef = m_shaderFactory.Phong("__PhongShader");

    MaterialRef materialRef = m_materialResource.create(phongShaderRef);

    m_materialResource.setUniform(materialRef, "material.ambient", options.ambient);
    m_materialResource.setUniform(materialRef, "material.diffuse", options.diffuse);
    m_materialResource.setUniform(materialRef, "material.specular", options.specular);
    m_materialResource.setUniform(materialRef, "material.shininess", options.shininess);

    if (options.diffuseMap != 0)
        m_materialResource.setUniform(materialRef, "material.diffuseMap", options.diffuseMap);
    if (options.specularMap != 0)
        m_materialResource.setUniform(materialRef, "material.specularMap", options.specularMap);
    if (options.normalMap != 0)
        m_materialResource.setUniform(materialRef, "material.normalMap", options.normalMap);

    return materialRef;
}

MaterialRef MaterialFactory::Skybox(const SkyboxMaterialParameters& options)
{
    ShaderRef   skyboxShaderRef = m_shaderFactory.Skybox("__SkyboxShader");
    MaterialRef materialRef     = m_materialResource.create(skyboxShaderRef);

    if (options.colorMap != 0)
        m_materialResource.setUniform(materialRef, "material.colorMap", options.colorMap);

    return materialRef;
}