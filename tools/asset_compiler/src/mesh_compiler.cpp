#include "mesh_compiler.h"
#include <iostream>

void CompileMesh(const aiMesh *mesh, const char *outputPath)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position[0] = mesh->mVertices[i].x;
        vertex.position[1] = mesh->mVertices[i].y;
        vertex.position[2] = mesh->mVertices[i].z;
        vertex.normal[0] = mesh->mNormals[i].x;
        vertex.normal[1] = mesh->mNormals[i].y;
        vertex.normal[2] = mesh->mNormals[i].z;
        if (mesh->mTangents)
        {
            vertex.tangent[0] = mesh->mTangents[i].x;
            vertex.tangent[1] = mesh->mTangents[i].y;
            vertex.tangent[2] = mesh->mTangents[i].z;
        }
        if (mesh->mBitangents)
        {
            vertex.bitangent[0] = mesh->mBitangents[i].x;
            vertex.bitangent[1] = mesh->mBitangents[i].y;
            vertex.bitangent[2] = mesh->mBitangents[i].z;
        }
        if (mesh->mTextureCoords[0])
        {
            vertex.texCoord[0] = mesh->mTextureCoords[0][i].x;
            vertex.texCoord[1] = mesh->mTextureCoords[0][i].y;
        }
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    FILE *outputFile = fopen(outputPath, "wb");
    if (!outputFile)
    {
        std::cerr << "Failed to open output file: " << outputPath << std::endl;
        return;
    }

    MeshHeader header{
        0x4853454D, // 'MESH0'
        static_cast<uint32_t>(vertices.size()),
        static_cast<uint32_t>(indices.size())};

    fwrite(&header, sizeof(header), 1, outputFile);
    fwrite(vertices.data(), sizeof(Vertex), vertices.size(), outputFile);
    fwrite(indices.data(), sizeof(unsigned int), indices.size(), outputFile);
    fclose(outputFile);
}