#pragma once

#include <engine/model/Service.hpp>

#include <service/resource/MeshResource.hpp>
#include <utils/PrimitiveMeshes.hpp>

class MeshFactory : public Service
{
  public:
    MeshFactory(MeshResource& meshResource) : m_meshResource(meshResource) {}
    ~MeshFactory() override = default;

    MeshRef Cube()
    {
        return m_meshResource.create(kCubeVertices, kCubeIndices);
    }

  private:
    MeshResource& m_meshResource;
};