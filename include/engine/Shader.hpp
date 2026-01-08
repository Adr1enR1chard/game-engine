#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    static Shader FromFiles(const char *vertexPath, const char *fragmentPath);
    static Shader FromSource(const char *vertexSource, const char *fragmentSource);
    static Shader Default();

    void use() const;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 mat) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setDirectionalLight(const glm::vec3 &direction, const glm::vec3 &color, float intensity, float ambient) const;

private:
    Shader(unsigned int id) : ID(id) {};

    void checkCompileErrors(unsigned int shader, std::string type);

public:
    unsigned int ID;
};