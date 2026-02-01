#pragma once

#include <engine/Engine.hpp>

// --- Services ---
#include <service/factory/ShaderFactory.hpp>
#include <service/factory/MaterialFactory.hpp>
#include <service/factory/MeshFactory.hpp>
#include <service/factory/ModelFactory.hpp>
#include <service/factory/TextureFactory.hpp>

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
#include <component/CMeshRenderer.hpp>
#include <component/CPointLight.hpp>
#include <component/CTransform.hpp>

namespace default_bundle
{

  class DefaultBundle : public engine::Bundle
  {
  public:
    void install(engine::SystemRegistry &systems, engine::ServiceRegistry &services) const override;
    void uninstall(engine::SystemRegistry &systems, engine::ServiceRegistry &services) const override;
  };

} // namespace default_bundle