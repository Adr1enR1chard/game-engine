#pragma once

#include <glad/glad.h>

class Mesh
{
private:
    const float *vertices;
    const size_t vertexCount;

public:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

public:
    /// @brief Construct a new MeshHandle object
    /// @param vertices Pointer to the vertex data
    /// @param vertexCount Number of vertices
    Mesh(const float *vertices, size_t vertexCount);

    ~Mesh();

    const float *getVertices() const { return vertices; }
    unsigned int getVertexCount() const { return vertexCount; }
};
