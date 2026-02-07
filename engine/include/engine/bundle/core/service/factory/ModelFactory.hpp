#pragma once

#include <functional>
#include <engine/model/Service.hpp>
#include <engine/bundle/core/service/factory/MaterialFactory.hpp>
#include <engine/bundle/core/service/factory/MeshFactory.hpp>
#include <engine/bundle/core/service/factory/ShaderFactory.hpp>
#include <engine/bundle/core/service/factory/TextureFactory.hpp>

namespace engine
{

  struct ModelHandle
  {
    std::vector<MeshRef> meshRefs;
    std::vector<MaterialHandle> materials;

    void forEach(const std::function<void(MeshRef, MaterialHandle &, size_t)> &func)
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

    ModelHandle LoadModel(const char *modelPath);

  private:
    MaterialFactory &m_materialFactory;
    ShaderFactory &m_shaderFactory;
    TextureFactory &m_textureFactory;
    MeshFactory &m_meshFactory;
  };

} // namespace engine