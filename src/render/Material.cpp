#include <engine/Material.hpp>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

constexpr const char* kDefaultVertexShader = R"( 
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 vUV;
out vec3 vWorldPos;
out vec3 vWorldNormal;
out mat3 vTBN;

// ------ WORLD UNIFORMS ------
// -- Set in the RenderSystem -
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// -----------------------------

void main()
{
    vUV = aUV;
    vWorldPos = vec3(model * vec4(aPos, 1.0));
    vWorldNormal = normalize(mat3(transpose(inverse(model))) * aNormal);
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    vTBN = mat3(T, B, N);
}
)";

constexpr const char* kDefaultFragmentShader = R"( 
#version 330 core

struct DirLight {
    vec3 direction;
    vec3 color;
    float intensity;
};  

struct PointLight {    
    vec3 position;
    vec3 color;
    float intensity;
}; 

struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    sampler2D albedoMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;
    sampler2D normalMap;
};

out vec4 FragColor;

in vec2 vUV;
in vec3 vWorldPos;
in vec3 vWorldNormal;
in mat3 vTBN;

// ------ MATERIAL UNIFORMS -------
// --- Require defaults value per -
uniform Material material;
// -------------------------------

// ------ LIGHT UNIFORMS ------
uniform DirLight dirLight;
uniform PointLight pointLights[32]; // Max 32 point lights
uniform int pointLightCount;
// -----------------------------

// ------ WORLD UNIFORM ------
uniform vec3 viewPos; // Set in the LightSystem
// ----------------------------
const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 
    
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 CalcDirLight(DirLight light, vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, float ao)
{
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L);

    vec3 radiance = light.color * light.intensity;
    
    vec3 F0 = vec3(0.04); 
    F0      = mix(F0, albedo, metallic);
    vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);

    float NDF = DistributionGGX(N, H, roughness);       
    float G   = GeometrySmith(N, V, L, roughness);  
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)  + 0.0001;
    vec3 specular     = numerator / denominator; 
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    
    kD *= 1.0 - metallic;
  
    float NdotL = max(dot(N, L), 0.0);        
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 CalcPointLight(PointLight light, vec3 N, vec3 worldPos, vec3 V, vec3 albedo, float metallic, float roughness, float ao)
{
    vec3 L = normalize(light.position - worldPos);
    vec3 H = normalize(V + L);

    float distance    = length(light.position - worldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance    = light.color * light.intensity * attenuation;
    
    vec3 F0 = vec3(0.04); 
    F0      = mix(F0, albedo, metallic);
    vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);

    float NDF = DistributionGGX(N, H, roughness);       
    float G   = GeometrySmith(N, V, L, roughness);  
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)  + 0.0001;
    vec3 specular     = numerator / denominator; 
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    
    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);        
    return (kD * albedo / PI + specular) * radiance * NdotL;
} 

void main()
{
    vec3 N = texture(material.normalMap, vUV).rgb;
    N = normalize(vTBN * (N * 2.0 - 1.0));
    vec3 V = normalize(viewPos - vWorldPos);

    vec3 albedo     = pow(texture(material.albedoMap, vUV).rgb * material.albedo, vec3(2.2));
    float metallic  = texture(material.metallicMap, vUV).r * material.metallic;
    float roughness = texture(material.roughnessMap, vUV).r * material.roughness;
    float ao        = texture(material.aoMap, vUV).r * material.ao;
    
    vec3 Lo = CalcDirLight(dirLight, N, V, albedo, metallic, roughness, ao);
    for (int i = 0; i < pointLightCount; ++i) {
        Lo += CalcPointLight(pointLights[i], N, vWorldPos, V, albedo, metallic, roughness, ao);
    }

    vec3 ambient = vec3(0.03) * material.albedo * material.ao;
    vec3 color   = ambient + Lo;  
    
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}
)";

std::shared_ptr<Material> Material::m_defaultMaterial = nullptr;

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

std::shared_ptr<Material> Material::Default()
{
    if (m_defaultMaterial != nullptr) {
        return m_defaultMaterial;
    }

    m_defaultMaterial = Material::FromSource(kDefaultVertexShader, kDefaultFragmentShader);
    return m_defaultMaterial;
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