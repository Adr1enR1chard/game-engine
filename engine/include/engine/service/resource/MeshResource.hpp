#pragma once
#include <engine/model/Service.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <queue>
#include <unordered_map>
#include <engine/utils/IdManager.hpp>
#include <engine/utils/RenderTypes.hpp>
#include <vector>

#include <assets_format/model_format.hpp>

/// @brief Service responsible for creating and managing mesh resources.
class MeshResource : public Service
{
public:
  MeshResource() = default;
  ~MeshResource() override;

public:
  MeshRef create(std::vector<VertexLayout> vertices, std::vector<unsigned int> indices, glm::mat4 localModel = glm::mat4(1.0f));

  void remove(MeshRef meshRef);
  void draw(MeshRef meshRef) const;

  glm::mat4 getLocalModel(MeshRef meshRef) const;

private:
  /// @brief Internal structure to hold mesh data. Depends on the graphics API used.
  struct MeshData;

  struct MeshDataDeleter
  {
    void operator()(MeshData *meshData);
  };

private:
  IdManager m_idManager;
  std::unordered_map<MeshRef, std::unique_ptr<MeshData, MeshDataDeleter>> m_meshes;
};