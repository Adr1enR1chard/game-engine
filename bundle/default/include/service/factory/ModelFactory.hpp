#pragma once

#include <engine/model/Service.hpp>
#include <service/factory/MaterialFactory.hpp>
#include <service/factory/MeshFactory.hpp>
#include <service/factory/ShaderFactory.hpp>
#include <service/factory/TextureFactory.hpp>
#include <engine/service/resource/ModelResource.hpp>

namespace default_bundle
{

  class ModelFactory : public engine::Service
  {
  public:
    ModelFactory(MaterialFactory &materialFactory, ShaderFactory &shaderFactory, TextureFactory &textureFactory,
                 MeshFactory &meshFactory, engine::ModelResource &modelResource)
        : m_materialFactory(materialFactory), m_shaderFactory(shaderFactory), m_textureFactory(textureFactory),
          m_meshFactory(meshFactory), m_modelResource(modelResource)
    {
    }
    ~ModelFactory() override = default;

    engine::ModelRef LoadModel(const char *modelPath);

  private:
    MaterialFactory &m_materialFactory;
    ShaderFactory &m_shaderFactory;
    TextureFactory &m_textureFactory;
    MeshFactory &m_meshFactory;
    engine::ModelResource &m_modelResource;
  };

} // namespace default_bundle