#include <engine/bundle/core/service/factory/ShaderFactory.hpp>

#include <fstream>
#include <sstream>
#include <engine/utils/Log.hpp>

namespace engine
{
    ShaderRef ShaderFactory::CustomShader(const char *name, const char *vertexShaderPath, const char *fragmentShaderPath,
                                          const engine::UniformCollection &defaultUniforms, const Renderer::ShaderParameters &params)
    {
        if (ShaderRef existingShaderRef = m_renderer.getShaderByName(name); existingShaderRef != 0)
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

        return m_renderer.allocateShader({name, vertexShaderCode.c_str(), fragmentShaderCode.c_str(), defaultUniforms, params});
    }

    ShaderRef ShaderFactory::PBRShader(const char *name)
    {
        if (ShaderRef existingShaderRef = m_renderer.getShaderByName(name); existingShaderRef != 0)
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
                                {"material.baseColorMap", TextureUniform{m_textureFactory.WhiteTexture2D(), Texture2D}},
                                {"material.normalMap", TextureUniform{m_textureFactory.NormalTexture2D(), Texture2D}},
                                {"material.metallicMap", TextureUniform{m_textureFactory.BlackTexture2D(), Texture2D}},
                                {"material.roughnessMap", TextureUniform{m_textureFactory.WhiteTexture2D(), Texture2D}},
                                {"material.aoMap", TextureUniform{m_textureFactory.WhiteTexture2D(), Texture2D}},
                            });
    }

    ShaderRef ShaderFactory::PhongShader(const char *name)
    {
        if (ShaderRef existingShaderRef = m_renderer.getShaderByName(name); existingShaderRef != 0)
        {
            return existingShaderRef;
        }

        return CustomShader(name, "default-bundle-assets/shaders/Default.vert", "default-bundle-assets/shaders/Phong.frag",
                            {
                                {"material.ambient", glm::vec3(0.1f, 0.1f, 0.1f)},
                                {"material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f)},
                                {"material.specular", glm::vec3(1.0f, 1.0f, 1.0f)},
                                {"material.shininess", 32.0f},
                                {"material.diffuseMap", TextureUniform{m_textureFactory.WhiteTexture2D(), Texture2D}},
                                {"material.specularMap", TextureUniform{m_textureFactory.WhiteTexture2D(), Texture2D}},
                                {"material.normalMap", TextureUniform{m_textureFactory.NormalTexture2D(), Texture2D}},
                            });
    }

    ShaderRef ShaderFactory::SkyboxShader(const char *name)
    {
        if (ShaderRef existingShaderRef = m_renderer.getShaderByName(name); existingShaderRef != 0)
        {
            return existingShaderRef;
        }

        return CustomShader(
            name, "default-bundle-assets/shaders/Skybox.vert", "default-bundle-assets/shaders/Skybox.frag",
            {
                {"material.colorMap",
                 TextureUniform{m_textureFactory.CubeMap({"default-bundle-assets/textures/skybox/right.jpg", "default-bundle-assets/textures/skybox/left.jpg",
                                                          "default-bundle-assets/textures/skybox/top.jpg", "default-bundle-assets/textures/skybox/bottom.jpg",
                                                          "default-bundle-assets/textures/skybox/front.jpg", "default-bundle-assets/textures/skybox/back.jpg"}),
                                CubeMap}},
            },
            {
                .enableBackfaceCulling = false,
                .enableDepthTest = false,
                .enableDepthWrite = false,
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

} // namespace engine