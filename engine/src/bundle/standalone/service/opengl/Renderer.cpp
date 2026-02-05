#include "engine/bundle/standalone/service/Renderer.hpp"
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
        GLuint textureAttachment = 0;
    };
    void Renderer::FramebufferDataDeleter::operator()(FramebufferData *framebufferData)
    {
        if (framebufferData)
        {
            glDeleteFramebuffers(1, &framebufferData->fbo);
            if (framebufferData->textureAttachment)
            {
                glDeleteTextures(1, &framebufferData->textureAttachment);
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

    FramebufferRef Renderer::allocateDepthFramebuffer(unsigned int width, unsigned int height, bool withBorder)
    {
        FramebufferRef newFramebufferRef = m_framebuffersId.alloc();
        auto framebufferData = std::unique_ptr<FramebufferData, FramebufferDataDeleter>(new FramebufferData());

        glGenFramebuffers(1, &framebufferData->fbo);

        glBindFramebuffer(GL_FRAMEBUFFER, framebufferData->fbo);
        glGenTextures(1, &framebufferData->textureAttachment);
        glBindTexture(GL_TEXTURE_2D, framebufferData->textureAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        if (withBorder)
        {
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, framebufferData->textureAttachment, 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Log::Print("Failed to create framebuffer.", LogLevel::Error);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        auto err = glGetError();
        if (err != GL_NO_ERROR)
        {
            Log::Print("OpenGL error creating depth framebuffer: 0x" + std::to_string(err), LogLevel::Error);
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
            m_textures.erase(it);
            m_texturesId.free(texture);
        }
    }

    void Renderer::freeMesh(MeshRef mesh)
    {
        auto it = m_meshes.find(mesh);
        if (it != m_meshes.end())
        {
            m_meshes.erase(it);
            m_meshesId.free(mesh);
        }
    }

    void Renderer::freeShader(ShaderRef shader)
    {
        auto it = m_shaders.find(shader);
        if (it != m_shaders.end())
        {
            m_shaders.erase(it);
            m_shadersId.free(shader);
        }
    }

    void Renderer::freeFramebuffer(FramebufferRef framebuffer)
    {
        auto it = m_framebuffers.find(framebuffer);
        if (it != m_framebuffers.end())
        {
            m_framebuffers.erase(it);
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

    void Renderer::setFramebuffer(FramebufferRef framebuffer)
    {
        // Retrieve the current framebuffer
        GLint currentFramebuffer = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFramebuffer);

        auto currentFramebufferData = std::unique_ptr<FramebufferData, FramebufferDataDeleter>(new FramebufferData({static_cast<GLuint>(currentFramebuffer), 0}));
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
                    else if constexpr (std::is_same_v<T, TextureUniform>)
                    {
                        if (m_textures.find(v.textureRef) == m_textures.end())
                        {
                            Log::Print("Texture not found for textureRef " + std::to_string(v.textureRef) + " out of " + std::to_string(m_textures.size()),
                                       LogLevel::Warning);
                            return;
                        }
                        glActiveTexture(GL_TEXTURE0 + textureUnit);
                        switch (v.type)
                        {
                        case Texture2D:
                            glBindTexture(GL_TEXTURE_2D, m_textures[v.textureRef]->textureID);
                            break;
                        case CubeMap:
                            glBindTexture(GL_TEXTURE_CUBE_MAP, m_textures[v.textureRef]->textureID);
                            break;
                        }
                        glUniform1i(location, textureUnit);
                        textureUnit++;
                    }
                    else if constexpr (std::is_same_v<T, FramebufferUniform>)
                    {
                        if (m_framebuffers.find(v.framebufferRef) == m_framebuffers.end())
                        {
                            Log::Print("Renderer::ApplyUniforms - Framebuffer not found for framebufferRef " + std::to_string(v.framebufferRef) + " out of " + std::to_string(m_framebuffers.size()),
                                       LogLevel::Warning);
                            return;
                        }
                        glActiveTexture(GL_TEXTURE0 + textureUnit);
                        glBindTexture(GL_TEXTURE_2D, m_framebuffers[v.framebufferRef]->textureAttachment);
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