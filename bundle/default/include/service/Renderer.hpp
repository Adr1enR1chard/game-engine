#pragma once

#include <engine/model/Service.hpp>

#include <model/Mesh.hpp>

class Renderer : public Service
{
  public:
    Renderer()           = default;
    ~Renderer() override = default;

  public:
    void loadMesh(const Mesh& mesh);
    void drawMesh(unsigned int VAO, unsigned int indexCount) const;

  private:
    std::unordered_map<MeshId, unsigned int> m_loadedMeshes;
};