/// @file MeshResource.cpp
/// @brief Implementation of the MeshResource service using OpenGL.

#include <glad/glad.h>
#include <service/resource/MeshResource.hpp>
#include <engine/utils/Log.hpp>

struct MeshResource::MeshData
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int indicesCount;
    glm::mat4 localModel = glm::mat4(1.0f);
};

void MeshResource::MeshDataDeleter::operator()(MeshData *meshData)
{
    if (meshData)
    {
        glDeleteVertexArrays(1, &meshData->VAO);
        glDeleteBuffers(1, &meshData->VBO);
        glDeleteBuffers(1, &meshData->EBO);
        delete meshData;
    }
}

MeshResource::~MeshResource() = default;

MeshRef MeshResource::create(std::vector<Vertex> vertices, std::vector<Index> indices, glm::mat4 localModel)
{
    MeshRef newMeshRef = m_idManager.alloc();
    auto meshData = std::unique_ptr<MeshData, MeshDataDeleter>(new MeshData());

    meshData->indicesCount = static_cast<unsigned int>(indices.size());
    meshData->localModel = localModel;

    // Generate OpenGL buffers
    glGenVertexArrays(1, &meshData->VAO);
    glGenBuffers(1, &meshData->VBO);
    glGenBuffers(1, &meshData->EBO);

    glBindVertexArray(meshData->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, meshData->VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
    // vertex tangents
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));
    // vertex bitangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bitangent));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_meshes.emplace(newMeshRef, std::move(meshData));

    return newMeshRef;
}

void MeshResource::remove(MeshRef meshRef)
{
    auto it = m_meshes.find(meshRef);
    if (it != m_meshes.end())
    {
        m_meshes.erase(it);
        m_idManager.free(meshRef);
        // Note : The MeshDataDeleter will be called automatically to free OpenGL resources.
    }
}

void MeshResource::draw(MeshRef meshRef) const
{
    auto it = m_meshes.find(meshRef);
    if (it != m_meshes.end())
    {
        glBindVertexArray(it->second->VAO);
        glDrawElements(GL_TRIANGLES, it->second->indicesCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

// TODO: Don't store localModel but either compute it according to the node hierarchy dynamically or store the full transform hierarchy.
glm::mat4 MeshResource::getLocalModel(MeshRef meshRef) const
{
    auto it = m_meshes.find(meshRef);
    if (it != m_meshes.end())
    {
        return it->second->localModel;
    }
    return glm::mat4(1.0f);
}