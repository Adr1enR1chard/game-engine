#pragma once

#include <memory>

class Mesh
{
private:
    const float *vertices;
    const size_t vertexCount;

private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    friend class RenderSystem;

public:
    /// @brief Construct a new MeshHandle object
    /// @param vertices Pointer to the vertex data
    /// @param vertexCount Number of vertices
    Mesh(const float *vertices, size_t vertexCount);

    static std::shared_ptr<Mesh> Cube();

    ~Mesh();

    const float *getVertices() const { return vertices; }
    unsigned int getVertexCount() const { return vertexCount; }
};
