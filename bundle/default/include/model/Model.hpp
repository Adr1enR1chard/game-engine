#pragma once
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <model/Mesh.hpp>
#include <model/Texture.hpp>

class Model
{
  public:
    Model(const char* modelPath)
    {
        loadModel(std::string(modelPath));
    }
    Model()  = default;
    ~Model() = default;

    void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 modelMatrix) const;

    std::shared_ptr<Mesh> getMesh(unsigned int index) const
    {
        return meshes[index];
    }

    unsigned int getMeshCount() const
    {
        return meshes.size();
    }

  private:
    std::vector<std::shared_ptr<Mesh>>             meshes;
    std::vector<std::shared_ptr<MaterialInstance>> materials;
    std::unordered_map<uint16_t, uint16_t>         meshMaterialMap;
    std::unordered_map<uint32_t, uint16_t>         assimpMaterialMap;
    std::string                                    modelDirectory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform = glm::mat4(1.0f));
    std::shared_ptr<Mesh>    processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& nodeTransform);
    std::unique_ptr<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};