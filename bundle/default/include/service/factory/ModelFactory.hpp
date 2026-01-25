#pragma once

#include <engine/model/Service.hpp>
#include <service/factory/MaterialFactory.hpp>
#include <service/factory/MeshFactory.hpp>
#include <service/factory/ShaderFactory.hpp>
#include <service/factory/TextureFactory.hpp>
#include <service/resource/ModelResource.hpp>

class ModelFactory : public Service
{
  public:
    ModelFactory(MaterialFactory& materialFactory, ShaderFactory& shaderFactory, TextureFactory& textureFactory,
                 MeshFactory& meshFactory, ModelResource& modelResource)
        : m_materialFactory(materialFactory), m_shaderFactory(shaderFactory), m_textureFactory(textureFactory),
          m_meshFactory(meshFactory), m_modelResource(modelResource)
    {
    }
    ~ModelFactory() override = default;

    ModelRef LoadModel(const char* modelPath);

  private:
    std::vector<MeshMaterialBinding> processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform,
                                                 const char* modelPath);
    MeshRef                          processMesh(aiMesh* mesh, const glm::mat4& transform);
    MaterialRef                      processMaterial(aiMesh* mesh, const aiScene* scene, const char* modelPath);
    std::optional<TextureRef>        loadMaterialTextures(aiMaterial* mat, aiTextureType type, const char* modelPath);

  private:
    MaterialFactory& m_materialFactory;
    ShaderFactory&   m_shaderFactory;
    TextureFactory&  m_textureFactory;
    MeshFactory&     m_meshFactory;
    ModelResource&   m_modelResource;
};