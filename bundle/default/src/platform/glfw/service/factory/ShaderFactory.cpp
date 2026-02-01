#include <service/factory/ShaderFactory.hpp>

#include <fstream>
#include <sstream>
#include <engine/utils/Log.hpp>

namespace default_bundle
{
    using namespace engine;

    ShaderRef ShaderFactory::CustomShader(const char *name, const char *vertexShaderPath, const char *fragmentShaderPath,
                                          const UniformCollection &defaultUniforms, const ShaderParameters &params)
    {
        if (ShaderRef existingShaderRef = m_shaderResource.get(name); existingShaderRef != 0)
        {
            return existingShaderRef;
        }

        std::string vertexShaderCode;
        loadShaderFromFile(vertexShaderPath, vertexShaderCode);
        if (vertexShaderCode.empty())
        {
            Log::Print("Failed to load vertex shader from file: " + std::string(vertexShaderPath), LogLevel::Critical);
            return 0;
        }

        std::string fragmentShaderCode;
        loadShaderFromFile(fragmentShaderPath, fragmentShaderCode);
        if (fragmentShaderCode.empty())
        {
            Log::Print("Failed to load fragment shader from file: " + std::string(fragmentShaderPath), LogLevel::Critical);
            return 0;
        }

        return m_shaderResource.create(name, vertexShaderCode.c_str(), fragmentShaderCode.c_str(), defaultUniforms, params);
    }

    ShaderRef ShaderFactory::PBRShader(const char *name)
    {
        if (ShaderRef existingShaderRef = m_shaderResource.get(name); existingShaderRef != 0)
        {
            return existingShaderRef;
        }

        return CustomShader(name, "default-bundle-assets/shaders/Default.vert", "default-bundle-assets/shaders/PBR.frag",
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

    ShaderRef ShaderFactory::PhongShader(const char *name)
    {
        if (ShaderRef existingShaderRef = m_shaderResource.get(name); existingShaderRef != 0)
        {
            return existingShaderRef;
        }

        return CustomShader(name, "default-bundle-assets/shaders/Default.vert", "default-bundle-assets/shaders/Phong.frag",
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

    ShaderRef ShaderFactory::SkyboxShader(const char *name)
    {
        if (ShaderRef existingShaderRef = m_shaderResource.get(name); existingShaderRef != 0)
        {
            return existingShaderRef;
        }

        return CustomShader(
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

    void ShaderFactory::loadShaderFromFile(const char *filePath, std::string &outShaderCode)
    {
        std::ifstream shaderFile;
        shaderFile.open(filePath);
        if (!shaderFile.is_open())
        {
            Log::Print("Failed to open shader file: " + std::string(filePath), LogLevel::Critical);
            return;
        }

        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        outShaderCode = shaderStream.str();
    }

} // namespace default_bundle