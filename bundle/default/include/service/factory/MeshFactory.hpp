#pragma once

#include <engine/model/Service.hpp>

#include <engine/service/resource/MeshResource.hpp>

class MeshFactory : public Service
{
public:
  MeshFactory(MeshResource &meshResource) : m_meshResource(meshResource) {}
  ~MeshFactory() override = default;

  MeshRef Raw(const std::vector<VertexLayout> &vertices, const std::vector<unsigned int> &indices,
              glm::mat4 localModel = glm::mat4(1.0f))
  {
    return m_meshResource.create(vertices, indices, localModel);
  }

  MeshRef Cube();

  MeshRef Sphere(float radius = 0.5f, unsigned int sectorCount = 36, unsigned int stackCount = 18);

  MeshRef Plane();

private:
  MeshResource &m_meshResource;
};