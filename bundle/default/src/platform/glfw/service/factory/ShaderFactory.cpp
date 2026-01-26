#include <service/factory/ShaderFactory.hpp>

ShaderRef ShaderFactory::PBR(const char *name)
{
    if (ShaderRef existingShaderRef = m_shaderResource.get(name); existingShaderRef != 0)
    {
        return existingShaderRef;
    }

    return m_shaderResource.create(name, "default-bundle-assets/shaders/Default.vert", "default-bundle-assets/shaders/PBR.frag",
                                   {
                                       {"material.baseColor", glm::vec3(1.0f, 1.0f, 1.0f)},
                                       {"material.metallic", 1.0f},
                                       {"material.roughness", 1.0f},
                                       {"material.ao", 1.0f},
                                       {"material.useMetallicRoughnessMap", false},
                                       {"material.baseColorMap", m_textureFactory.WhiteTexture2D()},
                                       {"material.normalMap", m_textureFactory.NormalTexture2D()},
                                       {"material.metallicMap", m_textureFactory.BlackTexture2D()},
                                       {"material.roughnessMap", m_textureFactory.WhiteTexture2D()},
                                       {"material.aoMap", m_textureFactory.WhiteTexture2D()},
                                   });
}

ShaderRef ShaderFactory::Phong(const char *name)
{
    if (ShaderRef existingShaderRef = m_shaderResource.get(name); existingShaderRef != 0)
    {
        return existingShaderRef;
    }

    return m_shaderResource.create(name, "default-bundle-assets/shaders/Default.vert", "default-bundle-assets/shaders/Phong.frag",
                                   {
                                       {"material.ambient", glm::vec3(0.1f, 0.1f, 0.1f)},
                                       {"material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f)},
                                       {"material.specular", glm::vec3(1.0f, 1.0f, 1.0f)},
                                       {"material.shininess", 32.0f},
                                       {"material.diffuseMap", m_textureFactory.WhiteTexture2D()},
                                       {"material.specularMap", m_textureFactory.WhiteTexture2D()},
                                       {"material.normalMap", m_textureFactory.NormalTexture2D()},
                                   });
}

ShaderRef ShaderFactory::Skybox(const char *name)
{
    if (ShaderRef existingShaderRef = m_shaderResource.get(name); existingShaderRef != 0)
    {
        return existingShaderRef;
    }

    return m_shaderResource.create(
        name, "default-bundle-assets/shaders/Skybox.vert", "default-bundle-assets/shaders/Skybox.frag",
        {
            {"material.colorMap",
             m_textureFactory.CubeMap({"default-bundle-assets/textures/skybox/right.jpg", "default-bundle-assets/textures/skybox/left.jpg",
                                       "default-bundle-assets/textures/skybox/top.jpg", "default-bundle-assets/textures/skybox/bottom.jpg",
                                       "default-bundle-assets/textures/skybox/front.jpg", "default-bundle-assets/textures/skybox/back.jpg"})},
        },
        {
            .cullFaceEnabled = false,
            .depthTestEnabled = false,
            .depthWriteEnabled = false,
        });
}