#pragma once

#include "engine/model/Service.hpp"
#include "engine/utils/RenderTypes.hpp"
#include "engine/utils/IdManager.hpp"
#include "assets_format/texture_format.hpp"
#include "assets_format/model_format.hpp"
#include <memory>
#include <string>
#include <glm/glm.hpp>

namespace engine
{
    /// @brief Service responsible for rendering operations.
    class Renderer : public Service
    {
    public:
        Renderer() = default;
        ~Renderer() override = default;

        struct TextureAttributes
        {
            unsigned int width;
            unsigned int height;
            unsigned int channels;
            TextureFormat format;
            std::vector<unsigned char> data;
        };

        struct MeshAttributes
        {
            std::vector<VertexLayout> vertices;
            std::vector<unsigned int> indices;
            glm::mat4 localModel = glm::mat4(1.0f);
        };

        struct ShaderParameters
        {
            bool enableBackfaceCulling = true;
            bool enableBlending = false;
            bool enableDepthTest = true;
            bool enableDepthWrite = true;
            bool enableScissorTest = false;
            bool enableStencilTest = false;
            bool enableWireframe = false;
        };

        struct ShaderAttributes
        {
            std::string name;
            std::string vertexShaderSource;
            std::string fragmentShaderSource;
            const UniformCollection &defaultUniforms;
            const ShaderParameters &params;
        };

        TextureRef allocateTexture2D(const TextureAttributes &texture);
        TextureRef allocateCubeMap(const std::vector<TextureAttributes> &faces);
        MeshRef allocateMesh(const MeshAttributes &mesh);
        ShaderRef allocateShader(const ShaderAttributes &shader);
        FramebufferRef allocateFramebuffer(unsigned int width, unsigned int height, bool withColorAttachment, bool withDepthAttachment);

        void freeTexture(TextureRef texture);
        void freeMesh(MeshRef mesh);
        void freeShader(ShaderRef shader);
        void freeFramebuffer(FramebufferRef framebuffer);

        ShaderRef getShaderByName(const std::string &name) const;

        glm::mat4 getLocalModel(MeshRef mesh) const;
        void drawMesh(MeshRef mesh, ShaderRef shader, const UniformCollection &uniforms);

    private:
#pragma region Data Structures
        struct TextureData;
        struct TextureDataDeleter
        {
            void operator()(TextureData *textureData);
        };
        IdManager m_texturesId;
        std::unordered_map<TextureRef, std::unique_ptr<TextureData, TextureDataDeleter>> m_textures;

        struct MeshData;
        struct MeshDataDeleter
        {
            void operator()(MeshData *meshData);
        };
        IdManager m_meshesId;
        std::unordered_map<MeshRef, std::unique_ptr<MeshData, MeshDataDeleter>> m_meshes;

        struct ShaderData;
        struct ShaderDataDeleter
        {
            void operator()(ShaderData *shaderData);
        };
        IdManager m_shadersId;
        std::unordered_map<ShaderRef, std::unique_ptr<ShaderData, ShaderDataDeleter>> m_shaders;
        std::unordered_map<std::string, ShaderRef> m_nameToShaderRef;

        struct FramebufferData;
        struct FramebufferDataDeleter
        {
            void operator()(FramebufferData *framebufferData);
        };
        IdManager m_framebuffersId;
        std::unordered_map<FramebufferRef, std::unique_ptr<FramebufferData, FramebufferDataDeleter>> m_framebuffers;
#pragma endregion

        void applyUniforms(const std::unique_ptr<ShaderData, ShaderDataDeleter> &shaderData, const UniformCollection &uniforms);
        void applyShaderParameters(const std::unique_ptr<ShaderData, ShaderDataDeleter> &shaderData);
    };
}