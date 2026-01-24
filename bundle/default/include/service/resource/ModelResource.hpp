#pragma once

#include <assimp/scene.h>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <engine/model/Service.hpp>
#include <service/resource/MaterialResource.hpp>
#include <service/resource/MeshResource.hpp>
#include <service/resource/ShaderResource.hpp>
#include <utils/IdManager.hpp>
#include <utils/RenderTypes.hpp>

class ModelResource : public Service
{
  public:
    ModelResource(MeshResource& meshResource, MaterialResource& materialResource, ShaderResource& shaderResource,
                  TextureResource& textureResource)
        : m_meshResource(meshResource), m_materialResource(materialResource), m_shaderResource(shaderResource),
          m_textureResource(textureResource)
    {
    }
    ~ModelResource() override = default;

    ModelRef create(const char* modelPath);

    void forEach(ModelRef modelRef, const std::function<void(MeshRef, MaterialRef, size_t)>& func) const;

  private:
    struct MeshMaterialBinding {
        MeshRef     meshRef;
        MaterialRef materialRef;
    };

    std::vector<MeshMaterialBinding> processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
    MeshRef                          processMesh(aiMesh* mesh, const glm::mat4& transform);
    MaterialRef                      processMaterial(aiMesh* mesh, const aiScene* scene);
    std::optional<TextureRef>        loadMaterialTextures(aiMaterial* mat, aiTextureType type);

  private:
    IdManager                                                      m_idManager;
    std::unordered_map<ModelRef, std::vector<MeshMaterialBinding>> m_modelMeshes;

    MeshResource&     m_meshResource;
    MaterialResource& m_materialResource;
    ShaderResource&   m_shaderResource;
    TextureResource&  m_textureResource;
};