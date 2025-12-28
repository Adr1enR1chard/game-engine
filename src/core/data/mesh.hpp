#ifndef __MESH_HPP__
#define __MESH_HPP__

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
    /// @brief Construct a new Mesh object
    /// @param vertices Pointer to the vertex data
    /// @param vertexCount Number of vertices
    Mesh(const float *vertices, size_t vertexCount) : vertices(vertices), vertexCount(vertexCount)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    ~Mesh()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    const float *getVertices() const { return vertices; }
    unsigned int getVertexCount() const { return vertexCount; }
};

#endif