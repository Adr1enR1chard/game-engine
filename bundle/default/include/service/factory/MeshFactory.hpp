#pragma once

#include <engine/model/Service.hpp>

#include <engine/service/resource/MeshResource.hpp>

namespace default_bundle
{

  class MeshFactory : public engine::Service
  {
  public:
    MeshFactory(engine::MeshResource &meshResource) : m_meshResource(meshResource) {}
    ~MeshFactory() override = default;

    engine::MeshRef Raw(const std::vector<VertexLayout> &vertices, const std::vector<unsigned int> &indices,
                        glm::mat4 localModel = glm::mat4(1.0f))
    {
      return m_meshResource.create(vertices, indices, localModel);
    }

    engine::MeshRef Cube();

    engine::MeshRef Sphere(float radius = 0.5f, unsigned int sectorCount = 36, unsigned int stackCount = 18);

    engine::MeshRef Plane();

  private:
    engine::MeshResource &m_meshResource;
  };

} // namespace default_bundle