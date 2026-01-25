#pragma once

#include <engine/Engine.hpp>

// --- Services ---
#include <service/Input.hpp>
#include <service/Window.hpp>
#include <service/factory/MaterialFactory.hpp>
#include <service/factory/MeshFactory.hpp>
#include <service/factory/ModelFactory.hpp>
#include <service/factory/ShaderFactory.hpp>
#include <service/factory/TextureFactory.hpp>
#include <service/resource/MaterialResource.hpp>
#include <service/resource/MeshResource.hpp>
#include <service/resource/ModelResource.hpp>
#include <service/resource/ShaderResource.hpp>
#include <service/resource/TextureResource.hpp>

// --- Systems ---
#include <system/CameraSystem.hpp>
#include <system/EnvironmentSystem.hpp>
#include <system/LightSystem.hpp>
#include <system/PlatformSystem.hpp>
#include <system/RenderSystem.hpp>
#include <system/TransformSystem.hpp>

// --- Components ---
#include <component/CCamera.hpp>
#include <component/CDirectionalLight.hpp>
#include <component/CEnvironment.hpp>
#include <component/CMeshRenderer.hpp>
#include <component/CPointLight.hpp>
#include <component/CTransform.hpp>

class DefaultBundle : public Bundle
{
  public:
    void install(SystemRegistry& systems, ServiceRegistry& services) const override;
    void uninstall(SystemRegistry& systems, ServiceRegistry& services) const override;
};