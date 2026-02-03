#pragma once

#include <functional>
#include <engine/model/Service.hpp>
#include <service/factory/MaterialFactory.hpp>
#include <service/factory/MeshFactory.hpp>
#include <service/factory/ShaderFactory.hpp>
#include <service/factory/TextureFactory.hpp>

namespace default_bundle
{

  using namespace engine;

  struct Model
  {
    std::vector<MeshRef> meshRefs;
    std::vector<Material> materials;

    void forEach(const std::function<void(MeshRef, Material &, size_t)> &func)
    {
      for (size_t i = 0; i < meshRefs.size(); ++i)
      {
        func(meshRefs[i], materials[i], i);
      }
    }
  };

  class ModelFactory : public Service
  {
  public:
    ModelFactory(MaterialFactory &materialFactory, ShaderFactory &shaderFactory, TextureFactory &textureFactory,
                 MeshFactory &meshFactory)
        : m_materialFactory(materialFactory), m_shaderFactory(shaderFactory), m_textureFactory(textureFactory),
          m_meshFactory(meshFactory)
    {
    }
    ~ModelFactory() override = default;

    Model LoadModel(const char *modelPath);

  private:
    MaterialFactory &m_materialFactory;
    ShaderFactory &m_shaderFactory;
    TextureFactory &m_textureFactory;
    MeshFactory &m_meshFactory;
  };

} // namespace default_bundle