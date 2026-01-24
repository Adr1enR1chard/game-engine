#pragma once

#include <memory>

#include <engine/model/Service.hpp>
#include <service/resource/MeshResource.hpp>
#include <utils/IdManager.hpp>
#include <utils/RenderTypes.hpp>

class ModelResource : public Service
{
  public:
    ModelResource(MeshResource& meshResource) : m_meshResource(meshResource) {}
    ~ModelResource() override = default;

    ModelRef create(const char* modelPath);

    void forEach(ModelRef modelRef, const std::function<void(MeshRef)>& func) const;

  private:
    IdManager                                              m_idManager;
    std::unordered_map<ModelRef, std::vector<MeshRef>>     m_modelMeshes;
    std::unordered_map<ModelRef, std::vector<MaterialRef>> m_modelMaterials;
    MeshResource&                                          m_meshResource;
};