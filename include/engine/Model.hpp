#pragma once
#include "Mesh.hpp"
#include "Texture.hpp"
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Model
{
  public:
    Model(const char* path)
    {
        loadModel(path);
    }
    Model()  = default;
    ~Model() = default;

    void Draw(class MaterialInstance& materialInstance, glm::mat4 modelMatrix) const;

    std::shared_ptr<Mesh> getMesh(unsigned int index) const
    {
        return meshes[index];
    }

    unsigned int getMeshCount() const
    {
        return meshes.size();
    }

  private:
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::string                        directory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform = glm::mat4(1.0f));
    std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& nodeTransform);
    std::vector<Texture>  loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};