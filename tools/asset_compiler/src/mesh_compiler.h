#pragma once

#include <cstdint>
#include <assimp/scene.h>

struct Vertex
{
    float position[3];
    float normal[3];
    float texCoord[2];
    float tangent[3];
    float bitangent[3];
};

struct MeshHeader
{
    uint32_t magic; // 'MESH0'
    uint32_t vertexCount;
    uint32_t indexCount;
};

void CompileMesh(const aiMesh *mesh, const char *outputPath);