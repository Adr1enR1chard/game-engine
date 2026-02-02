#include <engine/service/resource/ShaderResource.hpp>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <engine/utils/Log.hpp>

#include <engine/service/resource/TextureResource.hpp>

/// @brief Macro to execute code within a specific OpenGL shader program context
#define SCOPED_GL_PROGRAM(programID)                       \
    GLint __prevProgram;                                   \
    if (1)                                                 \
    {                                                      \
        glGetIntegerv(GL_CURRENT_PROGRAM, &__prevProgram); \
        glUseProgram(programID);                           \
        goto __end_scoped_gl_program;                      \
    }                                                      \
    else                                                   \
        while (1)                                          \
            if (1)                                         \
            {                                              \
                glUseProgram(__prevProgram);               \
                break;                                     \
            }                                              \
            else                                           \
            __end_scoped_gl_program:

namespace engine
{

    struct ShaderResource::ShaderImpl
    {
        unsigned int programID = 0;
    };

    void ShaderResource::ShaderImplDeleter::operator()(ShaderImpl *shaderImpl)
    {
        if (shaderImpl)
        {
            glDeleteProgram(shaderImpl->programID);
            delete shaderImpl;
        }
    }

    inline unsigned int compileShader(const char *vertexSource, const char *fragmentSource)
    {
        unsigned int vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexSource, NULL);
        glCompileShader(vertex);

        int success;
        char infoLog[512];
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            Log::Print("Vertex Shader Compilation Error: " + std::string(infoLog), LogLevel::Critical);
            return 0;
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentSource, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            Log::Print("Fragment Shader Compilation Error: " + std::string(infoLog), LogLevel::Critical);
            return 0;
        }

        unsigned int program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            Log::Print("Shader Program Linking Error: " + std::string(infoLog), LogLevel::Critical);
            return 0;
        }

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return program;
    }

    ShaderRef ShaderResource::create(const char *name, const char *vertexShaderSource,
                                     const char *fragmentShaderSource, const UniformCollection &defaultUniforms,
                                     const ShaderParameters &params)
    {
        if (m_nameToShaderRef.find(name) != m_nameToShaderRef.end())
        {
            return m_nameToShaderRef[name];
        }

        unsigned int shaderProgram = compileShader(vertexShaderSource, fragmentShaderSource);
        ShaderRef newShaderRef = m_idManager.alloc();
        m_nameToShaderRef[name] = newShaderRef;
        m_loadedShaders[newShaderRef] = std::unique_ptr<ShaderData>(new ShaderData(
            {std::unique_ptr<ShaderImpl, ShaderImplDeleter>(new ShaderImpl{shaderProgram}), defaultUniforms, params}));
        return newShaderRef;
    }

    ShaderRef ShaderResource::get(const char *name) const
    {
        auto it = m_nameToShaderRef.find(name);
        if (it != m_nameToShaderRef.end())
        {
            return it->second;
        }
        return 0;
    }

    void ShaderResource::applyUniforms(ShaderRef shaderRef, const UniformCollection *uniforms,
                                       const TextureResource &textureResource) const
    {
        auto it = m_loadedShaders.find(shaderRef);
        if (it == m_loadedShaders.end())
            return;

        int shaderProgram = it->second->impl->programID;
        SCOPED_GL_PROGRAM(shaderProgram)
        {
            unsigned int textureUnit = 0;
            UniformCollection combinedUniforms = it->second->defaultUniforms;
            if (uniforms)
            {
                for (const auto &[name, value] : *uniforms)
                {
                    combinedUniforms[name] = value;
                }
            }
            for (const auto &[name, value] : combinedUniforms)
            {
                uint32_t location = glGetUniformLocation(it->second->impl->programID, name.c_str());
                if (location == static_cast<uint32_t>(-1))
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
                        else if constexpr (std::is_same_v<T, TextureRef>)
                        {
                            glActiveTexture(GL_TEXTURE0 + textureUnit);
                            textureResource.bind(v);
                            glUniform1i(location, textureUnit);
                            textureUnit++;
                        }
                    },
                    value);
            }
        }
    }

    void ShaderResource::setParameters(ShaderRef shaderRef, const ShaderParameters &params)
    {
        auto it = m_loadedShaders.find(shaderRef);
        if (it == m_loadedShaders.end())
            return;

        it->second->parameters = params;
    }

    void ShaderResource::setUniform(ShaderRef shaderRef, const std::string &uniformName, const UniformValue &value) const
    {
        auto it = m_loadedShaders.find(shaderRef);
        if (it == m_loadedShaders.end())
            return;

        unsigned int shaderProgram = it->second->impl->programID;
        SCOPED_GL_PROGRAM(shaderProgram)
        {
            uint32_t location = glGetUniformLocation(it->second->impl->programID, uniformName.c_str());
            if (location == static_cast<uint32_t>(-1))
            {
                return;
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
                },
                value);
        }
    }

    void ShaderResource::bind(ShaderRef shaderRef, glm::mat4 viewMatrix, glm::mat4 projectionMatrix,
                              glm::mat4 modelMatrix) const
    {
        auto it = m_loadedShaders.find(shaderRef);
        if (it == m_loadedShaders.end())
            return;

        unsigned int shaderProgram = it->second->impl->programID;
        glUseProgram(shaderProgram);

        uint32_t viewLoc = glGetUniformLocation(shaderProgram, "view");
        uint32_t projLoc = glGetUniformLocation(shaderProgram, "projection");
        uint32_t modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        // Set rendering states
        const ShaderParameters &params = it->second->parameters;
        if (params.cullFaceEnabled)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(params.backfaceCulling ? GL_BACK : GL_FRONT);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
        if (params.depthTestEnabled)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
        if (params.depthWriteEnabled)
        {
            glDepthMask(GL_TRUE);
        }
        else
        {
            glDepthMask(GL_FALSE);
        }
        if (params.blendEnabled)
        {
            glEnable(GL_BLEND);
        }
        else
        {
            glDisable(GL_BLEND);
        }
        if (params.wireframeEnabled)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        if (params.scissorEnabled)
        {
            glEnable(GL_SCISSOR_TEST);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }
        if (params.stencilEnabled)
        {
            glEnable(GL_STENCIL_TEST);
        }
        else
        {
            glDisable(GL_STENCIL_TEST);
        }
    }

} // namespace engine
