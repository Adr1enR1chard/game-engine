#pragma once

#include <engine/model/Service.hpp>
#include <service/factory/MaterialFactory.hpp>
#include <service/factory/MeshFactory.hpp>
#include <service/factory/ShaderFactory.hpp>
#include <service/factory/TextureFactory.hpp>
#include <service/resource/ModelResource.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class ModelFactory : public Service
{
public:
  ModelFactory(MaterialFactory &materialFactory, ShaderFactory &shaderFactory, TextureFactory &textureFactory,
               MeshFactory &meshFactory, ModelResource &modelResource)
      : m_materialFactory(materialFactory), m_shaderFactory(shaderFactory), m_textureFactory(textureFactory),
        m_meshFactory(meshFactory), m_modelResource(modelResource)
  {
  }
  ~ModelFactory() override = default;

  ModelRef LoadModel(const char *modelPath);

private:
  MaterialFactory &m_materialFactory;
  ShaderFactory &m_shaderFactory;
  TextureFactory &m_textureFactory;
  MeshFactory &m_meshFactory;
  ModelResource &m_modelResource;
};