#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <assimp/scene.h>

struct ModelMesh
{
    uint32_t meshHash;
    uint32_t materialHash;
};

struct ModelMaterial
{
    uint32_t baseColorHash;
    uint32_t metallicHash;
    uint32_t roughnessHash;
    uint32_t normalHash;
    uint32_t aoHash;
};

struct ModelHeader
{
    uint32_t magic; // 'MODL0'
    uint32_t meshCount;
    uint32_t materialCount;
};

void CompileModel(const char *inputPath, const char *outputPath);

void ProcessNode(const aiNode *node, const aiScene *scene, const glm::mat4 &parentTransform, const char *outputPath, int &meshCounter);

void ProcessMesh(const aiMesh *mesh, const aiScene *scene, const glm::mat4 &transform, const char *outputPath, int meshIndex);

void ProcessMaterial(const aiMaterial *material, const char *outputPath);
void ProcessTexture(const aiString &texturePath, const char *outputPath);