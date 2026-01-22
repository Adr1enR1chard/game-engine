#pragma once
#include <engine/model/Service.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <queue>
#include <unordered_map>
#include <utils/RenderTypes.hpp>
#include <vector>

/// @brief Service responsible for creating and managing mesh resources.
class MeshResource : public Service
{
  public:
    MeshResource() = default;
    ~MeshResource();

  public:
    MeshRef create(std::vector<Vertex> vertices, std::vector<Index> indices);
    MeshRef createCube();

    void remove(MeshRef meshRef);

  private:
    friend class RenderSystem;

    void      draw(MeshRef meshRef) const;
    glm::mat4 getLocalModel(MeshRef meshRef) const;

  private:
    /// @brief Internal structure to hold mesh data. Depends on the graphics API used.
    struct MeshData;

    struct MeshDataDeleter {
        void operator()(MeshData* meshData);
    };

    MeshRef alloc();
    void    clear();

    MeshRef                                                                 m_nextMeshId = 1;
    std::queue<MeshRef>                                                     m_freeMeshIds;
    std::unordered_map<MeshRef, std::unique_ptr<MeshData, MeshDataDeleter>> m_meshes;
};