#pragma once
#include "Mesh.hpp"
#include "Texture.hpp"
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model(const char *path)
    {
        loadModel(path);
    }
    Model() = default;
    ~Model() = default;

    void Draw(class Shader &shader, glm::mat4 modelMatrix) const;

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
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene, const glm::mat4 &parentTransform = glm::mat4(1.0f));
    std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                              std::string typeName);
};