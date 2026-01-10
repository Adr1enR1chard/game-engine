#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    static Shader Default();
    ~Shader();

    void use() const;
    void setDirectionalLight(const glm::vec3 &direction, const glm::vec3 &color, float intensity, float ambient) const;
    bool setBool(const std::string &name, bool value) const;
    bool setInt(const std::string &name, int value) const;
    bool setFloat(const std::string &name, float value) const;
    bool setMat4(const std::string &name, const glm::mat4 mat) const;
    bool setVec3(const std::string &name, const glm::vec3 &value) const;

private:
    static Shader FromFiles(const char *vertexPath, const char *fragmentPath);
    static Shader FromSource(const char *vertexSource, const char *fragmentSource);

    Shader(unsigned int id) : ID(id) {};

    void checkCompileErrors(unsigned int shader, std::string type);

    static unsigned int defaultShaderID;

public:
    unsigned int ID;
};