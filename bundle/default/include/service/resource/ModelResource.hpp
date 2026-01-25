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

struct MeshMaterialBinding {
    MeshRef     meshRef;
    MaterialRef materialRef;
};

class ModelResource : public Service
{
  public:
    ModelResource()           = default;
    ~ModelResource() override = default;

    ModelRef create(std::vector<MeshMaterialBinding> meshMaterialBindings);

    void forEach(ModelRef modelRef, const std::function<void(MeshRef, MaterialRef, size_t)>& func) const;

  private:
    IdManager                                                      m_idManager;
    std::unordered_map<ModelRef, std::vector<MeshMaterialBinding>> m_modelMeshes;
};