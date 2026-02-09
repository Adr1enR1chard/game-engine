#include "engine/bundle/core/service/Renderer.hpp"
#include <engine/utils/Log.hpp>
#include "assets_format/opengl_format.hpp"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace engine
{
#pragma region Data Structures
    struct Renderer::TextureData
    {
        GLuint textureID = 0;
        TextureType type = Texture2D;
    };
    void Renderer::TextureDataDeleter::operator()(TextureData *textureData)
    {
        if (textureData)
        {
            glDeleteTextures(1, &textureData->textureID);
            delete textureData;
        }
    }

    struct Renderer::MeshData
    {
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;
        size_t indexCount = 0;
        glm::mat4 localModel = glm::mat4(1.0f);
    };
    void Renderer::MeshDataDeleter::operator()(MeshData *meshData)
    {
        if (meshData)
        {
            glDeleteVertexArrays(1, &meshData->vao);
            glDeleteBuffers(1, &meshData->vbo);
            glDeleteBuffers(1, &meshData->ebo);
            delete meshData;
        }
    }

    struct Renderer::ShaderData
    {
        GLint programID = 0;
        UniformCollection defaultUniforms = {};
        ShaderParameters params = {};
    };
    void Renderer::ShaderDataDeleter::operator()(ShaderData *shaderData)
    {
        if (shaderData)
        {
            glDeleteProgram(shaderData->programID);
            delete shaderData;
        }
    }

    struct Renderer::FramebufferData
    {
        GLuint fbo = 0;
        std::vector<TextureRef> colorAttachments;
        TextureRef depthAttachment = 0;
        TextureRef stencilAttachment = 0;
    };
    void Renderer::FramebufferDataDeleter::operator()(FramebufferData *framebufferData)
    {
        if (framebufferData)
        {
            glDeleteFramebuffers(1, &framebufferData->fbo);
            if (framebufferData->depthAttachment)
            {
                glDeleteTextures(1, &framebufferData->depthAttachment);
            }
            for (auto colorAttachment : framebufferData->colorAttachments)
            {
                glDeleteTextures(1, &colorAttachment);
            }
            if (framebufferData->stencilAttachment)
            {
                glDeleteTextures(1, &framebufferData->stencilAttachment);
            }
            delete framebufferData;
        }
    }
#pragma endregion

#pragma region Allocation
    TextureRef Renderer::allocateTexture2D(const TextureAttributes &texture)
    {
        if (!OpenGLFormat::IsCompressed(texture.format))
        {
            Log::Print("Texture format is not compressed. Use the asset compiler to compress the texture.", LogLevel::Error);
            return 0;
        }

        TextureRef newTextureRef = m_texturesId.alloc();
        auto textureData = std::unique_ptr<TextureData, TextureDataDeleter>(new TextureData());
        textureData->type = Texture2D;

        GLuint glFormat = OpenGLFormat::GetOpenGLFormat(
            texture.format,
            texture.channels == 4);

        glGenTextures(1, &textureData->textureID);
        glBindTexture(GL_TEXTURE_2D, textureData->textureID);

        glCompressedTexImage2D(
            GL_TEXTURE_2D,
            0,
            glFormat,
            texture.width,
            texture.height,
            0,
            texture.data.size(),
            texture.data.data());

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        m_textures[newTextureRef] = std::move(textureData);
        return newTextureRef;
    }

    TextureRef Renderer::allocateCubeMap(const std::vector<TextureAttributes> &faces)
    {
        TextureRef newTextureRef = m_texturesId.alloc();
        auto textureData = std::unique_ptr<TextureData, TextureDataDeleter>(new TextureData());
        textureData->type = CubeMap;

        glGenTextures(1, &textureData->textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureData->textureID);

        for (unsigned int i = 0; i < faces.size(); i++)
        {
            if (!OpenGLFormat::IsCompressed(faces[i].format))
            {
                Log::Print("CubeMap face format is not compressed. Use the asset compiler to compress the texture.", LogLevel::Error);
                return 0;
            }

            GLuint glFormat = OpenGLFormat::GetOpenGLFormat(
                faces[i].format,
                faces[i].channels == 4);

            glCompressedTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                glFormat,
                faces[i].width,
                faces[i].height,
                0,
                faces[i].data.size(),
                faces[i].data.data());
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        m_textures[newTextureRef] = std::move(textureData);
        return newTextureRef;
    }

    MeshRef Renderer::allocateMesh(const MeshAttributes &mesh)
    {
        MeshRef newMeshRef = m_meshesId.alloc();
        auto meshData = std::unique_ptr<MeshData, MeshDataDeleter>(new MeshData());

        meshData->indexCount = static_cast<unsigned int>(mesh.indices.size());
        meshData->localModel = mesh.localModel;

        glGenVertexArrays(1, &meshData->vao);
        glGenBuffers(1, &meshData->vbo);
        glGenBuffers(1, &meshData->ebo);

        glBindVertexArray(meshData->vao);
        glBindBuffer(GL_ARRAY_BUFFER, meshData->vbo);

        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(VertexLayout), mesh.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLayout), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLayout), (void *)offsetof(VertexLayout, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexLayout), (void *)offsetof(VertexLayout, texCoord));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLayout), (void *)offsetof(VertexLayout, tangent));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexLayout), (void *)offsetof(VertexLayout, bitangent));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_meshes.emplace(newMeshRef, std::move(meshData));

        return newMeshRef;
    }

    ShaderRef Renderer::allocateShader(const ShaderAttributes &shader)
    {
        if (m_nameToShaderRef.find(shader.name) != m_nameToShaderRef.end())
        {
            return m_nameToShaderRef[shader.name];
        }

        auto shaderData = std::unique_ptr<ShaderData, ShaderDataDeleter>(new ShaderData());

        int success;
        char infoLog[512];

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char *vertexSourceCStr = shader.vertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &vertexSourceCStr, NULL);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            Log::Print("Vertex Shader Compilation Error: " + std::string(infoLog), LogLevel::Critical);
            return 0;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fragmentSourceCStr = shader.fragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &fragmentSourceCStr, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            Log::Print("Fragment Shader Compilation Error: " + std::string(infoLog), LogLevel::Critical);
            return 0;
        }

        shaderData->programID = glCreateProgram();
        glAttachShader(shaderData->programID, vertexShader);
        glAttachShader(shaderData->programID, fragmentShader);
        glLinkProgram(shaderData->programID);
        glGetProgramiv(shaderData->programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderData->programID, 512, NULL, infoLog);
            Log::Print("Shader Program Linking Error: " + std::string(infoLog), LogLevel::Critical);
            return 0;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        if (!shaderData->programID)
        {
            Log::Print("Failed to compile and link shader: " + shader.name, LogLevel::Error);
            return 0;
        }

        shaderData->defaultUniforms = shader.defaultUniforms;
        shaderData->params = shader.params;

        ShaderRef newShaderRef = m_shadersId.alloc();
        m_shaders[newShaderRef] = std::move(shaderData);
        m_nameToShaderRef[shader.name] = newShaderRef;
        return newShaderRef;
    }

    FramebufferRef Renderer::allocateFramebuffer(unsigned int width, unsigned int height, int colorAttachments, bool withDepth, bool withStencil, bool drawBuffer, bool readBuffer)
    {
        FramebufferRef newFramebufferRef = m_framebuffersId.alloc();
        auto framebufferData = std::unique_ptr<FramebufferData, FramebufferDataDeleter>(new FramebufferData());

        glGenFramebuffers(1, &framebufferData->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferData->fbo);

        for (int i = 0; i < colorAttachments; i++)
        {
            TextureRef colorAttachmentRef = m_texturesId.alloc();
            auto colorTextureData = std::unique_ptr<TextureData, TextureDataDeleter>(new TextureData());
            colorTextureData->type = Texture2D;

            glGenTextures(1, &colorTextureData->textureID);
            glBindTexture(GL_TEXTURE_2D, colorTextureData->textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorTextureData->textureID, 0);

            m_textures[colorAttachmentRef] = std::move(colorTextureData);
            framebufferData->colorAttachments.push_back(colorAttachmentRef);
        }

        if (withDepth)
        {
            TextureRef depthAttachmentRef = m_texturesId.alloc();
            auto depthTextureData = std::unique_ptr<TextureData, TextureDataDeleter>(new TextureData());
            depthTextureData->type = Texture2D;

            glGenTextures(1, &depthTextureData->textureID);
            glBindTexture(GL_TEXTURE_2D, depthTextureData->textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureData->textureID, 0);

            m_textures[depthAttachmentRef] = std::move(depthTextureData);
            framebufferData->depthAttachment = depthAttachmentRef;
        }

        if (withStencil)
        {
            TextureRef stencilAttachmentRef = m_texturesId.alloc();
            auto stencilTextureData = std::unique_ptr<TextureData, TextureDataDeleter>(new TextureData());
            stencilTextureData->type = Texture2D;

            glGenTextures(1, &stencilTextureData->textureID);
            glBindTexture(GL_TEXTURE_2D, stencilTextureData->textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX8, width, height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilTextureData->textureID, 0);

            m_textures[stencilAttachmentRef] = std::move(stencilTextureData);
            framebufferData->stencilAttachment = stencilAttachmentRef;
        }

        if (!drawBuffer)
        {
            glDrawBuffer(GL_NONE);
        }
        if (!readBuffer)
        {
            glReadBuffer(GL_NONE);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Log::Print("Failed to create framebuffer.", LogLevel::Error);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            Log::Print("OpenGL error creating framebuffer: 0x" + std::to_string(err), LogLevel::Error);
        }

        m_framebuffers[newFramebufferRef] = std::move(framebufferData);
        return newFramebufferRef;
    }

#pragma endregion

#pragma region Deallocation
    void Renderer::freeTexture(TextureRef texture)
    {
        auto it = m_textures.find(texture);
        if (it != m_textures.end())
        {
            m_textures.erase(texture);
            m_texturesId.free(texture);
        }
    }

    void Renderer::freeMesh(MeshRef mesh)
    {
        auto it = m_meshes.find(mesh);
        if (it != m_meshes.end())
        {
            m_meshes.erase(mesh);
            m_meshesId.free(mesh);
        }
    }

    void Renderer::freeShader(ShaderRef shader)
    {
        auto it = m_shaders.find(shader);
        if (it != m_shaders.end())
        {
            m_shaders.erase(shader);
            m_shadersId.free(shader);
        }
    }

    void Renderer::freeFramebuffer(FramebufferRef framebuffer)
    {
        auto it = m_framebuffers.find(framebuffer);
        if (it != m_framebuffers.end())
        {
            m_framebuffers.erase(framebuffer);
            m_framebuffersId.free(framebuffer);
        }
    }

#pragma endregion

#pragma region Rendering
    ShaderRef Renderer::getShaderByName(const std::string &name) const
    {
        auto it = m_nameToShaderRef.find(name);
        if (it != m_nameToShaderRef.end())
        {
            return it->second;
        }
        return 0;
    }

    glm::mat4 Renderer::getLocalModel(MeshRef mesh) const
    {
        auto it = m_meshes.find(mesh);
        if (it != m_meshes.end())
        {
            return it->second->localModel;
        }
        Log::Print("Mesh not found for meshRef " + std::to_string(mesh) + " out of " + std::to_string(m_meshes.size()),
                   LogLevel::Warning);
        return glm::mat4(1.0f);
    }

    void Renderer::drawMesh(MeshRef mesh, ShaderRef shader, const UniformCollection &uniforms)
    {
        auto meshIt = m_meshes.find(mesh);
        if (meshIt == m_meshes.end())
        {
            Log::Print("Mesh not found for meshRef " + std::to_string(mesh) + " out of " + std::to_string(m_meshes.size()),
                       LogLevel::Warning);
            return;
        }

        auto shaderIt = m_shaders.find(shader);
        if (shaderIt == m_shaders.end())
        {
            Log::Print("Shader not found for shaderRef " + std::to_string(shader) + " out of " + std::to_string(m_shaders.size()),
                       LogLevel::Warning);
            return;
        }
        glUseProgram(shaderIt->second->programID);
        applyUniforms(shaderIt->second, uniforms);
        applyShaderParameters(shaderIt->second);
        glBindVertexArray(meshIt->second->vao);
        glDrawElements(GL_TRIANGLES, meshIt->second->indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
#pragma endregion

#pragma region State Management
    void Renderer::setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height, int *previousWidth, int *previousHeight)
    {
        if (previousWidth != nullptr && previousHeight != nullptr)
        {
            getViewportSize(*previousWidth, *previousHeight);
        }
        glViewport(x, y, width, height);
    }

    void Renderer::getViewportSize(int &width, int &height) const
    {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        width = viewport[2];
        height = viewport[3];
    }

    void Renderer::setScissorRect(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    {
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, y, width, height);
    }

    void Renderer::resetScissorRect()
    {
        glDisable(GL_SCISSOR_TEST);
    }

    TextureRef Renderer::getFramebufferColorAttachment(FramebufferRef framebuffer, size_t index) const
    {
        auto framebufferIt = m_framebuffers.find(framebuffer);
        if (framebufferIt == m_framebuffers.end())
        {
            Log::Print("Framebuffer not found for framebufferRef " + std::to_string(framebuffer) + " out of " + std::to_string(m_framebuffers.size()),
                       LogLevel::Warning);
            return 0;
        }
        if (index >= framebufferIt->second->colorAttachments.size())
        {
            Log::Print("Color attachment index out of range for framebufferRef " + std::to_string(framebuffer) + " with " + std::to_string(framebufferIt->second->colorAttachments.size()) + " color attachments",
                       LogLevel::Warning);
            return 0;
        }
        return framebufferIt->second->colorAttachments[index];
    }

    TextureRef Renderer::getFramebufferDepthAttachment(FramebufferRef framebuffer) const
    {
        auto framebufferIt = m_framebuffers.find(framebuffer);
        if (framebufferIt == m_framebuffers.end())
        {
            Log::Print("Framebuffer not found for framebufferRef " + std::to_string(framebuffer) + " out of " + std::to_string(m_framebuffers.size()),
                       LogLevel::Warning);
            return 0;
        }
        return framebufferIt->second->depthAttachment;
    }

    TextureRef Renderer::getFramebufferStencilAttachment(FramebufferRef framebuffer) const
    {
        auto framebufferIt = m_framebuffers.find(framebuffer);
        if (framebufferIt == m_framebuffers.end())
        {
            Log::Print("Framebuffer not found for framebufferRef " + std::to_string(framebuffer) + " out of " + std::to_string(m_framebuffers.size()),
                       LogLevel::Warning);
            return 0;
        }
        return framebufferIt->second->stencilAttachment;
    }

    void Renderer::setFramebuffer(FramebufferRef framebuffer)
    {
        // Retrieve the current framebuffer
        GLint currentFramebuffer = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFramebuffer);

        auto currentFramebufferData = std::unique_ptr<FramebufferData, FramebufferDataDeleter>(new FramebufferData({static_cast<GLuint>(currentFramebuffer)}));
        m_framebufferStack.push(std::move(currentFramebufferData));

        if (framebuffer != 0)
        {
            auto framebufferIt = m_framebuffers.find(framebuffer);
            if (framebufferIt == m_framebuffers.end())
            {
                Log::Print("Renderer::setFramebuffer - Framebuffer not found for framebufferRef " + std::to_string(framebuffer) + " out of " + std::to_string(m_framebuffers.size()),
                           LogLevel::Warning);
                return;
            }
            glBindFramebuffer(GL_FRAMEBUFFER, framebufferIt->second->fbo);
            GLsizei drawBuffersCount = static_cast<GLsizei>(framebufferIt->second->colorAttachments.size());
            std::vector<GLenum> drawBuffers(drawBuffersCount);
            for (GLsizei i = 0; i < drawBuffersCount; i++)
            {
                drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
            }
            if (drawBuffersCount > 0)
                glDrawBuffers(drawBuffersCount, drawBuffers.data());

            // checlk for gl error
            auto err = glGetError();
            if (err != GL_NO_ERROR)
            {
                Log::Print("OpenGL error binding framebuffer in Renderer::setFramebuffer: 0x" + std::to_string(err) + " for " + std::to_string(framebufferIt->second->fbo), LogLevel::Error);
            }
        }
        else
        {
            revertToPreviousFramebuffer();
        }
    }

    void Renderer::resetFramebuffer()
    {
        while (!m_framebufferStack.empty())
        {
            m_framebufferStack.pop();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            Log::Print("OpenGL error resetting framebuffer in Renderer::resetFramebuffer: 0x" + std::to_string(err), LogLevel::Error);
        }
    }

    void Renderer::revertToPreviousFramebuffer()
    {
        if (m_framebufferStack.empty())
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            Log::Print("Framebuffer stack is empty in Renderer::revertToPreviousFramebuffer, binding default framebuffer (0)", LogLevel::Warning);
            auto err = glGetError();
            if (err != GL_NO_ERROR)
            {
                Log::Print("OpenGL error binding default framebuffer in Renderer::revertToPreviousFramebuffer: 0x" + std::to_string(err), LogLevel::Error);
            }
            return;
        }

        auto previousFramebuffer = std::move(m_framebufferStack.top());
        glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer->fbo);

        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            Log::Print("OpenGL error binding previous framebuffer in Renderer::revertToPreviousFramebuffer: 0x" + std::to_string(err) + " for " + std::to_string(previousFramebuffer->fbo), LogLevel::Error);
        }

        m_framebufferStack.pop();
    }

    void Renderer::setTextureWrapMode(TextureRef texture, TextureWrapMode wrapModeS, TextureWrapMode wrapModeT, TextureWrapMode wrapModeR)
    {
        auto textureIt = m_textures.find(texture);
        if (textureIt == m_textures.end())
        {
            Log::Print("Texture not found for textureRef " + std::to_string(texture) + " out of " + std::to_string(m_textures.size()),
                       LogLevel::Warning);
            return;
        }

        GLenum glWrapModeS;
        switch (wrapModeS)
        {
        case Repeat:
            glWrapModeS = GL_REPEAT;
            break;
        case MirroredRepeat:
            glWrapModeS = GL_MIRRORED_REPEAT;
            break;
        case ClampToEdge:
            glWrapModeS = GL_CLAMP_TO_EDGE;
            break;
        case ClampToBorder:
            glWrapModeS = GL_CLAMP_TO_BORDER;
            break;
        default:
            Log::Print("Invalid wrap mode in Renderer::setTextureWrapMode", LogLevel::Error);
            return;
        }
        GLenum glWrapModeT;
        switch (wrapModeT)
        {
        case Repeat:
            glWrapModeT = GL_REPEAT;
            break;
        case MirroredRepeat:
            glWrapModeT = GL_MIRRORED_REPEAT;
            break;
        case ClampToEdge:
            glWrapModeT = GL_CLAMP_TO_EDGE;
            break;
        case ClampToBorder:
            glWrapModeT = GL_CLAMP_TO_BORDER;
            break;
        default:
            Log::Print("Invalid wrap mode in Renderer::setTextureWrapMode", LogLevel::Error);
            return;
        }
        GLenum glWrapModeR;
        switch (wrapModeR)
        {
        case Repeat:
            glWrapModeR = GL_REPEAT;
            break;
        case MirroredRepeat:
            glWrapModeR = GL_MIRRORED_REPEAT;
            break;
        case ClampToEdge:
            glWrapModeR = GL_CLAMP_TO_EDGE;
            break;
        case ClampToBorder:
            glWrapModeR = GL_CLAMP_TO_BORDER;
            break;
        default:
            Log::Print("Invalid wrap mode in Renderer::setTextureWrapMode", LogLevel::Error);
            return;
        }
        switch (textureIt->second->type)
        {
        case Texture2D:
            glBindTexture(GL_TEXTURE_2D, textureIt->second->textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrapModeS);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrapModeT);
            break;
        case CubeMap:
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureIt->second->textureID);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, glWrapModeS);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, glWrapModeT);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, glWrapModeR);
            break;
        }
    }

    void Renderer::setTextureFilterMode(TextureRef texture, TextureFilterMode minFilterMode, TextureFilterMode magFilterMode)
    {
        auto textureIt = m_textures.find(texture);
        if (textureIt == m_textures.end())
        {
            Log::Print("Texture not found for textureRef " + std::to_string(texture) + " out of " + std::to_string(m_textures.size()),
                       LogLevel::Warning);
            return;
        }

        GLenum glMinFilter;
        GLenum glMagFilter;
        switch (minFilterMode)
        {
        case Nearest:
            glMinFilter = GL_NEAREST;
            break;
        case Linear:
            glMinFilter = GL_LINEAR;
            break;
        case NearestMipmapNearest:
            glMinFilter = GL_NEAREST_MIPMAP_NEAREST;
            break;
        case LinearMipmapNearest:
            glMinFilter = GL_LINEAR_MIPMAP_NEAREST;
            break;
        case NearestMipmapLinear:
            glMinFilter = GL_NEAREST_MIPMAP_LINEAR;
            break;
        case LinearMipmapLinear:
            glMinFilter = GL_LINEAR_MIPMAP_LINEAR;
            break;
        default:
            Log::Print("Invalid filter mode in Renderer::setTextureFilterMode", LogLevel::Error);
            return;
        }

        switch (magFilterMode)
        {
        case Nearest:
            glMagFilter = GL_NEAREST;
            break;
        case Linear:
            glMagFilter = GL_LINEAR;
            break;
        case NearestMipmapNearest:
        case LinearMipmapNearest:
        case NearestMipmapLinear:
        case LinearMipmapLinear:
            Log::Print("Invalid mag filter mode in Renderer::setTextureFilterMode. Mipmapped filter modes are not valid for magnification.", LogLevel::Error);
            return;
        default:
            Log::Print("Invalid filter mode in Renderer::setTextureFilterMode", LogLevel::Error);
            return;
        }

        switch (textureIt->second->type)
        {
        case Texture2D:
            glBindTexture(GL_TEXTURE_2D, textureIt->second->textureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMinFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMagFilter);
            break;
        case CubeMap:
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureIt->second->textureID);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, glMinFilter);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, glMagFilter);
            break;
        }
    }

    void Renderer::setTextureBorderColor(TextureRef texture, const glm::vec4 &borderColor)
    {
        auto textureIt = m_textures.find(texture);
        if (textureIt == m_textures.end())
        {
            Log::Print("Texture not found for textureRef " + std::to_string(texture) + " out of " + std::to_string(m_textures.size()),
                       LogLevel::Warning);
            return;
        }

        switch (textureIt->second->type)
        {
        case Texture2D:
            glBindTexture(GL_TEXTURE_2D, textureIt->second->textureID);
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));
            break;
        case CubeMap:
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureIt->second->textureID);
            glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));
            break;
        }
    }

    void Renderer::clear(bool clearColor, bool clearDepth, bool clearStencil)
    {
        GLbitfield mask = 0;
        if (clearColor)
        {
            glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
            mask |= GL_COLOR_BUFFER_BIT;
        }
        if (clearDepth)
        {
            mask |= GL_DEPTH_BUFFER_BIT;
        }
        if (clearStencil)
        {
            mask |= GL_STENCIL_BUFFER_BIT;
        }
        glClear(mask);
    }

    void Renderer::enableMultisampling(bool enable)
    {
        if (enable)
        {
            glEnable(GL_MULTISAMPLE);
        }
        else
        {
            glDisable(GL_MULTISAMPLE);
        }
    }

    void Renderer::setClearColor(const glm::vec4 &color)
    {
        m_clearColor[0] = color.r;
        m_clearColor[1] = color.g;
        m_clearColor[2] = color.b;
        m_clearColor[3] = color.a;
        Log::Print("Set clear color to: (" + std::to_string(color.r) + ", " + std::to_string(color.g) + ", " + std::to_string(color.b) + ", " + std::to_string(color.a) + ")", LogLevel::Debug);
    }
#pragma endregion

#pragma region Helpers
    void Renderer::applyUniforms(const std::unique_ptr<ShaderData, ShaderDataDeleter> &shaderData, const UniformCollection &uniforms)
    {
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

        assert(currentProgram == shaderData->programID);

        UniformCollection combinedUniforms = shaderData->defaultUniforms;
        if (uniforms.size() > 0)
        {
            for (const auto &[name, value] : uniforms)
            {
                combinedUniforms[name] = value;
            }
        }

        unsigned int textureUnit = 0;
        for (const auto &[name, value] : combinedUniforms)
        {
            GLint location = glGetUniformLocation(shaderData->programID, name.c_str());
            if (location == -1)
            {
                continue;
            }
            std::visit(
                [&](auto const &v)
                {
                    using T = std::decay_t<decltype(v)>;

                    if constexpr (std::is_same_v<T, int>)
                    {
                        glUniform1i(location, v);
                    }
                    else if constexpr (std::is_same_v<T, float>)
                    {
                        glUniform1f(location, v);
                    }
                    else if constexpr (std::is_same_v<T, glm::vec2>)
                    {
                        glUniform2fv(location, 1, glm::value_ptr(v));
                    }
                    else if constexpr (std::is_same_v<T, glm::vec3>)
                    {
                        glUniform3fv(location, 1, glm::value_ptr(v));
                    }
                    else if constexpr (std::is_same_v<T, glm::vec4>)
                    {
                        glUniform4fv(location, 1, glm::value_ptr(v));
                    }
                    else if constexpr (std::is_same_v<T, glm::mat4>)
                    {
                        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(v));
                    }
                    else if constexpr (std::is_same_v<T, Sampler2D>)
                    {
                        if (m_textures.find(v.textureRef) == m_textures.end())
                        {
                            Log::Print("Texture not found for textureRef " + std::to_string(v.textureRef) + " out of " + std::to_string(m_textures.size()),
                                       LogLevel::Warning);
                            return;
                        }
                        glActiveTexture(GL_TEXTURE0 + textureUnit);
                        glBindTexture(GL_TEXTURE_2D, m_textures[v.textureRef]->textureID);
                        glUniform1i(location, textureUnit);
                        textureUnit++;
                    }
                    else if constexpr (std::is_same_v<T, SamplerCube>)
                    {
                        if (m_textures.find(v.textureRef) == m_textures.end())
                        {
                            Log::Print("Texture not found for textureRef " + std::to_string(v.textureRef) + " out of " + std::to_string(m_textures.size()),
                                       LogLevel::Warning);
                            return;
                        }
                        glActiveTexture(GL_TEXTURE0 + textureUnit);
                        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textures[v.textureRef]->textureID);
                        glUniform1i(location, textureUnit);
                        textureUnit++;
                    }
                },
                value);
        }
    }

    void Renderer::applyShaderParameters(const std::unique_ptr<ShaderData, ShaderDataDeleter> &shaderData)
    {
        if (shaderData->params.enableBackfaceCulling)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }

        if (shaderData->params.enableBlending)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }

        if (shaderData->params.enableDepthTest)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        glDepthMask(shaderData->params.enableDepthWrite ? GL_TRUE : GL_FALSE);

        if (shaderData->params.enableScissorTest)
        {
            glEnable(GL_SCISSOR_TEST);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }

        if (shaderData->params.enableStencilTest)
        {
            glEnable(GL_STENCIL_TEST);
        }
        else
        {
            glDisable(GL_STENCIL_TEST);
        }

        if (shaderData->params.enableWireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
#pragma endregion

} // namespace engine