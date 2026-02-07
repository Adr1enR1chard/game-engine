#pragma once

#include <engine/model/Bundle.hpp>

// --- Services ---
#include <service/factory/ShaderFactory.hpp>
#include <service/factory/MaterialFactory.hpp>
#include <service/factory/MeshFactory.hpp>
#include <service/factory/ModelFactory.hpp>
#include <service/factory/TextureFactory.hpp>
#include <service/ShadowMapping.hpp>

// --- Systems ---
#include <system/CameraSystem.hpp>
#include <system/EnvironmentSystem.hpp>
#include <system/LightSystem.hpp>
#include <system/RenderSystem.hpp>
#include <system/TransformSystem.hpp>

// --- Components ---
#include <component/CCamera.hpp>
#include <component/CDirectionalLight.hpp>
#include <component/CEnvironment.hpp>
#include <component/CMesh.hpp>
#include <component/CPointLight.hpp>
#include <component/CTransform.hpp>

#include <engine/bundle/core/CoreBundle.hpp>

namespace default_bundle
{

  using namespace engine;

  class DefaultBundle : public Bundle
  {
  public:
    void install(Engine &engine) const override;
    void uninstall(Engine &engine) const override;

    std::vector<std::type_index> getRequiredBundles() const override
    {
      return {std::type_index(typeid(CoreBundle))};
    }
  };

} // namespace default_bundle