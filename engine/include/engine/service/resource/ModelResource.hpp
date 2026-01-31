#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <engine/model/Service.hpp>
#include <engine/service/resource/MaterialResource.hpp>
#include <engine/service/resource/MeshResource.hpp>
#include <engine/service/resource/ShaderResource.hpp>
#include <engine/utils/IdManager.hpp>
#include <engine/utils/RenderTypes.hpp>
struct MeshMaterialBinding
{
  MeshRef meshRef;
  MaterialRef materialRef;
};

class ModelResource : public Service
{
public:
  ModelResource() = default;
  ~ModelResource() override = default;

  ModelRef create(std::vector<MeshMaterialBinding> meshMaterialBindings);

  void forEach(ModelRef modelRef, const std::function<void(MeshRef, MaterialRef, size_t)> &func) const;

private:
  IdManager m_idManager;
  std::unordered_map<ModelRef, std::vector<MeshMaterialBinding>> m_modelMeshes;
};