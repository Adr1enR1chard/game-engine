#include "model_compiler.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <filesystem>

#include "mesh_compiler.h"

glm::mat4 AiMatrixToGlmMat4(const aiMatrix4x4 &from)
{
    glm::mat4 to;
    to[0][0] = from.a1;
    to[1][0] = from.a2;
    to[2][0] = from.a3;
    to[3][0] = from.a4;
    to[0][1] = from.b1;
    to[1][1] = from.b2;
    to[2][1] = from.b3;
    to[3][1] = from.b4;
    to[0][2] = from.c1;
    to[1][2] = from.c2;
    to[2][2] = from.c3;
    to[3][2] = from.c4;
    to[0][3] = from.d1;
    to[1][3] = from.d2;
    to[2][3] = from.d3;
    to[3][3] = from.d4;
    return to;
}

void CompileModel(const char *inputPath, const char *outputPath)
{
    Assimp::Importer import;
    const aiScene *scene =
        import.ReadFile(inputPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Assimp error: " << import.GetErrorString() << std::endl;
        return;
    }

    int meshCounter = 0;
    ProcessNode(scene->mRootNode, scene, glm::mat4(1.0f), outputPath, meshCounter);

    ModelHeader header{
        0x4C444F4D, // 'MODL0'
        static_cast<uint32_t>(meshCounter),
        0 // materialCount placeholder
    };

    FILE *outputFile = fopen(outputPath, "wb");
    if (!outputFile)
    {
        std::cerr << "Failed to open output file for header writing: " << outputPath << std::endl;
        return;
    }
    fwrite(&header, sizeof(header), 1, outputFile);
    fclose(outputFile);
}

void ProcessNode(const aiNode *node, const aiScene *scene, const glm::mat4 &parentTransform, const char *outputPath, int &meshCounter)
{
    glm::mat4 nodeTransform = parentTransform * AiMatrixToGlmMat4(node->mTransformation);

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        std::cout << "Processing mesh " << i << " of node " << node->mName.C_Str() << std::endl;
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene, nodeTransform, outputPath, meshCounter++);
        // ProcessMaterial(scene->mMaterials[mesh->mMaterialIndex], outputPath);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene, nodeTransform, outputPath, meshCounter);
    }
}

void ProcessMesh(const aiMesh *mesh, const aiScene *scene, const glm::mat4 &transform, const char *outputPath, int meshIndex)
{
    std::string meshPath = (std::string(outputPath) + "." + (std::string("mesh_") + std::to_string(meshIndex) + ".asset"));

    CompileMesh(mesh, meshPath.c_str());
    std::cout << "Processed mesh with " << mesh->mNumVertices << " vertices at " << meshPath << std::endl;
}
