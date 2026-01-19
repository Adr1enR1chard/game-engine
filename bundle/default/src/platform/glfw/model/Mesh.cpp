#include <model/Mesh.hpp>

#include <model/MaterialInstance.hpp>

#include <glad/glad.h>
#include <iostream>

const int kVertexAttributeCount = 8;

const std::vector<Vertex> kCubeVertices = {
    // Back face (0, 0, -1)
    {{-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0, 0}, {1, 0, 0}, {0, 1, 0}},
    {{0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1, 0}, {1, 0, 0}, {0, 1, 0}},
    {{0.5f, 0.5f, -0.5f}, {0, 0, -1}, {1, 1}, {1, 0, 0}, {0, 1, 0}},
    {{-0.5f, 0.5f, -0.5f}, {0, 0, -1}, {0, 1}, {1, 0, 0}, {0, 1, 0}},

    // Front face (0, 0, 1)
    {{-0.5f, -0.5f, 0.5f}, {0, 0, 1}, {0, 0}, {1, 0, 0}, {0, 1, 0}},
    {{0.5f, -0.5f, 0.5f}, {0, 0, 1}, {1, 0}, {1, 0, 0}, {0, 1, 0}},
    {{0.5f, 0.5f, 0.5f}, {0, 0, 1}, {1, 1}, {1, 0, 0}, {0, 1, 0}},
    {{-0.5f, 0.5f, 0.5f}, {0, 0, 1}, {0, 1}, {1, 0, 0}, {0, 1, 0}},

    // Left face (-1, 0, 0)
    {{-0.5f, 0.5f, 0.5f}, {-1, 0, 0}, {1, 0}, {0, 1, 0}, {0, 0, -1}},
    {{-0.5f, 0.5f, -0.5f}, {-1, 0, 0}, {1, 1}, {0, 1, 0}, {0, 0, -1}},
    {{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {0, 1}, {0, 1, 0}, {0, 0, -1}},
    {{-0.5f, -0.5f, 0.5f}, {-1, 0, 0}, {0, 0}, {0, 1, 0}, {0, 0, -1}},

    // Right face (1, 0, 0)
    {{0.5f, 0.5f, 0.5f}, {1, 0, 0}, {1, 0}, {0, 1, 0}, {0, 0, -1}},
    {{0.5f, 0.5f, -0.5f}, {1, 0, 0}, {1, 1}, {0, 1, 0}, {0, 0, -1}},
    {{0.5f, -0.5f, -0.5f}, {1, 0, 0}, {0, 1}, {0, 1, 0}, {0, 0, -1}},
    {{0.5f, -0.5f, 0.5f}, {1, 0, 0}, {0, 0}, {0, 1, 0}, {0, 0, -1}},

    // Bottom face (0, -1, 0)
    {{-0.5f, -0.5f, -0.5f}, {0, -1, 0}, {0, 1}, {1, 0, 0}, {0, 0, 1}},
    {{0.5f, -0.5f, -0.5f}, {0, -1, 0}, {1, 1}, {1, 0, 0}, {0, 0, 1}},
    {{0.5f, -0.5f, 0.5f}, {0, -1, 0}, {1, 0}, {1, 0, 0}, {0, 0, 1}},
    {{-0.5f, -0.5f, 0.5f}, {0, -1, 0}, {0, 0}, {1, 0, 0}, {0, 0, 1}},

    // Top face (0, 1, 0)
    {{-0.5f, 0.5f, -0.5f}, {0, 1, 0}, {0, 1}, {1, 0, 0}, {0, 0, 1}},
    {{0.5f, 0.5f, -0.5f}, {0, 1, 0}, {1, 1}, {1, 0, 0}, {0, 0, 1}},
    {{0.5f, 0.5f, 0.5f}, {0, 1, 0}, {1, 0}, {1, 0, 0}, {0, 0, 1}},
    {{-0.5f, 0.5f, 0.5f}, {0, 1, 0}, {0, 0}, {1, 0, 0}, {0, 0, 1}},
};

const std::vector<uint32_t> kCubeIndices = {
    // Back face (-Z)
    0, 2, 1, 0, 3, 2,

    // Front face (+Z)
    4, 5, 6, 4, 6, 7,

    // Left face (-X)
    9, 10, 8, 10, 11, 8,

    // Right face (+X)
    14, 13, 12, 15, 14, 12,

    // Bottom face (-Y)
    17, 18, 16, 18, 19, 16,

    // Top face (+Y)
    22, 21, 20, 23, 22, 20};

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, const glm::mat4& localModel)
    : m_vertexCount(vertices.size()), m_indexCount(indices.size()), localModel(localModel)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    // vertex tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    // vertex bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::Draw(MaterialInstance& materialInstance, glm::mat4 modelMatrix) const
{
    materialInstance.link(modelMatrix * localModel);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::shared_ptr<Mesh> Mesh::Cube()
{
    return std::make_shared<Mesh>(kCubeVertices, kCubeIndices);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
