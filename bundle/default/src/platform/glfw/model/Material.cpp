#include <model/Material.hpp>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

std::shared_ptr<Material> Material::m_PBRMaterial   = nullptr;
std::shared_ptr<Material> Material::m_PhongMaterial = nullptr;

std::shared_ptr<Material> Material::FromFiles(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string   vertexCode;
    std::string   fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    return Material::FromSource(vShaderCode, fShaderCode);
}

std::shared_ptr<Material> Material::FromSource(const char* vertexSource, const char* fragmentSource)
{
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    // Check for compilation errors
    int  success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment ShaderHandle
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    // Check for compilation errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // shader Program
    unsigned int ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // Check for linking errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return std::make_shared<Material>(ID);
}

std::shared_ptr<Material> Material::PBR()
{
    if (m_PBRMaterial != nullptr) {
        return m_PBRMaterial;
    }

    m_PBRMaterial = Material::FromFiles("assets/shaders/Default.vert", "assets/shaders/PBR.frag");
    return m_PBRMaterial;
}

std::shared_ptr<Material> Material::Phong()
{
    if (m_PhongMaterial != nullptr) {
        return m_PhongMaterial;
    }

    m_PhongMaterial = Material::FromFiles("assets/shaders/Default.vert", "assets/shaders/Phong.frag");
    return m_PhongMaterial;
}

std::shared_ptr<Material> Material::Skybox()
{
    return Material::FromFiles("assets/shaders/Skybox.vert", "assets/shaders/Skybox.frag");
}

Material::~Material()
{
    glDeleteProgram(ID);
}

void Material::use(std::unordered_map<uint32_t, UniformValue>* uniforms,
                   std::unordered_map<std::string, Texture>* textures, glm::mat4 viewMatrix,
                   glm::mat4 projectionMatrix) const
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    if ((GLuint)current != ID)
        glUseProgram(ID);

    // Set standard uniforms
    setMat4("view", viewMatrix);
    setMat4("projection", projectionMatrix);

    // Set custom uniforms
    if (uniforms != nullptr) {
        for (const auto& [location, uniform] : *uniforms) {
            std::visit(
                [&](auto&& arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, int>) {
                        glUniform1i(location, arg);
                    } else if constexpr (std::is_same_v<T, float>) {
                        glUniform1f(location, arg);
                    } else if constexpr (std::is_same_v<T, glm::vec2>) {
                        glUniform2fv(location, 1, glm::value_ptr(arg));
                    } else if constexpr (std::is_same_v<T, glm::vec3>) {
                        glUniform3fv(location, 1, glm::value_ptr(arg));
                    } else if constexpr (std::is_same_v<T, glm::vec4>) {
                        glUniform4fv(location, 1, glm::value_ptr(arg));
                    } else if constexpr (std::is_same_v<T, glm::mat4>) {
                        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(arg));
                    }
                },
                uniform);
        }
    }

    // Bind textures
    if (textures != nullptr) {
        int textureUnit = 0;
        for (const auto& [name, texture] : *textures) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, texture.ID);
            setInt(name, textureUnit);
            textureUnit++;
        }
        glActiveTexture(GL_TEXTURE0);
    }
}

// void Material::use() const
// {
//     GLint current = 0;
//     glGetIntegerv(GL_CURRENT_PROGRAM, &current);
//     if ((GLuint)current != ID)
//         glUseProgram(ID);
// }

bool Material::setBool(const std::string& name, bool value) const
{
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform1i(location, (int)value);

    return location != -1;
}

bool Material::setInt(const std::string& name, int value) const
{
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform1i(location, value);

    return location != -1;
}

bool Material::setFloat(const std::string& name, float value) const
{
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform1f(location, value);

    return location != -1;
}

bool Material::setMat4(const std::string& name, const glm::mat4 mat) const
{
    int location = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));

    return location != -1;
}

bool Material::setVec3(const std::string& name, const glm::vec3& value) const
{
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(value));

    return location != -1;
}

void Material::setDirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity,
                                   float ambient) const
{
    glUniform3fv(glGetUniformLocation(ID, "dirLight.direction"), 1, glm::value_ptr(direction));
    glUniform3fv(glGetUniformLocation(ID, "dirLight.color"), 1, glm::value_ptr(color));
    glUniform1f(glGetUniformLocation(ID, "dirLight.intensity"), intensity);
    glUniform1f(glGetUniformLocation(ID, "dirLight.ambient"), ambient);
}

void Material::checkCompileErrors(unsigned int shader, std::string type)
{
    int  success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}