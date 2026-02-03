#pragma once

#include <engine/service/platform/Renderer.hpp>

namespace default_bundle
{
  using namespace engine;

  class MeshFactory : public Service
  {
  public:
    MeshFactory(Renderer &renderer) : m_renderer(renderer) {}
    ~MeshFactory() override = default;

    MeshRef Raw(const std::vector<VertexLayout> &vertices, const std::vector<unsigned int> &indices,
                glm::mat4 localModel = glm::mat4(1.0f))
    {
      return m_renderer.allocateMesh({vertices, indices, localModel});
    }

    MeshRef Cube();

    MeshRef Sphere(float radius = 0.5f, unsigned int sectorCount = 36, unsigned int stackCount = 18);

    MeshRef Plane();

  private:
    Renderer &m_renderer;
  };

} // namespace default_bundle