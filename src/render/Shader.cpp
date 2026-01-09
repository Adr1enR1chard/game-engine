#include <engine/Shader.hpp>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

constexpr const char *kDefaultVertexShader = R"( 
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec2 vUV;
out vec3 vNormal;
out vec3 vFragPos;

// ------ WORLD UNIFORMS ------
// -- Set in the RenderSystem -
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// -----------------------------

void main()
{
    vUV = aUV;
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vFragPos = vec3(model * vec4(aPos, 1.0));
    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

constexpr const char *kDefaultFragmentShader = R"( 
#version 330 core

struct DirLight {
    vec3 direction; 
    vec3 color;
    float ambient;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;
};

vec3 shadeDirLight(DirLight light, vec3 worldPos, vec3 N, vec3 viewPos, float shininess)
{
    vec3 V = normalize(viewPos - worldPos);

    vec3 L = normalize(-light.direction);

    float NdotL = max(dot(N, L), 0.0);

    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(R, V), 0.0), shininess);

    vec3 diffuse  = light.color * NdotL;
    vec3 specular = light.color * spec;
    vec3 ambient  = light.color * light.ambient;

    return ambient + (diffuse + specular) * light.intensity;
}

vec3 shadePointLight(PointLight light, vec3 worldPos, vec3 N, vec3 viewPos, float shininess)
{
    vec3 V = normalize(viewPos - worldPos);

    vec3 L = normalize(light.position - worldPos);
    float distance = length(light.position - worldPos);
    float attenuation = 1.0 / (1.0 + (distance / light.radius) * (distance / light.radius));

    float NdotL = max(dot(N, L), 0.0);

    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(R, V), 0.0), shininess);

    vec3 diffuse  = light.color * NdotL;
    vec3 specular = light.color * spec;

    return (diffuse + specular) * light.intensity * attenuation;
}

out vec4 FragColor;

in vec2 vUV;
in vec3 vNormal;
in vec3 vFragPos;

// ------ MATERIAL UNIFORMS -------
// --- Require defaults value per -
// --------  material -------------
uniform sampler2D albedoMap;
uniform vec3 albedo = vec3(1.0, 1.0, 1.0);
uniform float shininess = 12.0;
// -------------------------------

// ------ LIGHT UNIFORMS ------
uniform DirLight dirLight;
uniform PointLight pointLights[32]; // Max 32 point lights
uniform int pointLightCount;
// -----------------------------

// ------ WORLD UNIFORM ------
uniform vec3 viewPos; // Set in the LightSystem
// ----------------------------

void main()
{
    vec3 norm = normalize(vNormal);
    vec3 albedoColor = texture(albedoMap, vUV).rgb * albedo;
    vec3 lighting = shadeDirLight(dirLight, vFragPos, norm, viewPos, shininess);

    for (int i = 0; i < pointLightCount; ++i) {
        lighting += shadePointLight(pointLights[i], vFragPos, norm, viewPos, shininess);
    }

    FragColor = vec4(lighting * albedoColor, 1.0);
}
)";

unsigned int Shader::defaultShaderID = 0;

Shader Shader::FromFiles(const char *vertexPath, const char *fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
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
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    return Shader::FromSource(vShaderCode, fShaderCode);
}

Shader Shader::FromSource(const char *vertexSource, const char *fragmentSource)
{
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    // Check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // fragment ShaderHandle
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    // Check for compilation errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // shader Program
    unsigned int ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // Check for linking errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    Shader shader(ID);
    return shader;
}

Shader Shader::Default()
{
    if (defaultShaderID != 0)
    {
        return Shader(defaultShaderID);
    }

    Shader defaultShader = Shader::FromSource(kDefaultVertexShader, kDefaultFragmentShader);
    defaultShaderID = defaultShader.ID;
    return defaultShader;
}

void Shader::use() const
{
    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);
    if ((GLuint)current != ID)
        glUseProgram(ID);
}

bool Shader::setBool(const std::string &name, bool value) const
{
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform1i(location, (int)value);

    return location != -1;
}

bool Shader::setInt(const std::string &name, int value) const
{
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform1i(location, value);

    return location != -1;
}

bool Shader::setFloat(const std::string &name, float value) const
{
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform1f(location, value);

    return location != -1;
}

bool Shader::setMat4(const std::string &name, const glm::mat4 mat) const
{
    int location = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));

    return location != -1;
}

bool Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    int location = glGetUniformLocation(ID, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(value));

    return location != -1;
}

void Shader::setDirectionalLight(const glm::vec3 &direction, const glm::vec3 &color, float intensity, float ambient) const
{
    glUniform3fv(glGetUniformLocation(ID, "dirLight.direction"), 1, glm::value_ptr(direction));
    glUniform3fv(glGetUniformLocation(ID, "dirLight.color"), 1, glm::value_ptr(color));
    glUniform1f(glGetUniformLocation(ID, "dirLight.intensity"), intensity);
    glUniform1f(glGetUniformLocation(ID, "dirLight.ambient"), ambient);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}