#include <service/ShaderResource.hpp>

#include <engine/utils/Log.hpp>
#include <fstream>
#include <glad/glad.h>
#include <sstream>

inline void readShaderFiles(const char* vertexShaderPath, const char* fragmentShaderPath, std::string& vertexCode,
                            std::string& fragmentCode)
{
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);
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
        Log::Print("Error while reading shader files: " + std::string(e.what()), LogLevel::Critical);
    }
}

inline unsigned int compileShader(const char* vertexSource, const char* fragmentSource)
{

    unsigned int vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        Log::Print("Vertex Shader Compilation Error: " + std::string(infoLog), LogLevel::Critical);
        return 0;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        Log::Print("Fragment Shader Compilation Error: " + std::string(infoLog), LogLevel::Critical);
        return 0;
    }

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
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

ShaderRef ShaderResource::create(const char* name, const char* vertexShaderPath, const char* fragmentShaderPath)
{
    if (m_loadedShaders.find(name) != m_loadedShaders.end()) {
        return name;
    }

    std::string vertexCode, fragmentCode;
    readShaderFiles(vertexShaderPath, fragmentShaderPath, vertexCode, fragmentCode);
    const char* vertexSource   = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    if (unsigned int shaderProgram = compileShader(vertexSource, fragmentSource); shaderProgram != 0) {
        m_loadedShaders[name] = shaderProgram;
        return name;
    }
    return "";
}