#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <string>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    Mesh(const Mesh &) = delete;
    Mesh &operator=(const Mesh &) = delete;

    Mesh(Mesh &&other) noexcept
    {
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        vertexCount = other.vertexCount;
        indexCount = other.indexCount;
    }

    void Draw() const;

public:
    static std::shared_ptr<Mesh> Cube();

    ~Mesh();

    unsigned int getVertexCount() const { return vertexCount; }
    unsigned int getIndexCount() const { return indexCount; }

public:
    glm::mat4 localModel = glm::mat4(1.0f);

private:
    unsigned int vertexCount;
    unsigned int indexCount;

private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    friend class RenderSystem;
};
