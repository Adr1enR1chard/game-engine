#pragma once
#include <glm/glm.hpp>

#include <engine/model/System.hpp>
#include <engine/model/Entity.hpp>
#include <engine/bundle/core/component/CCamera.hpp>
#include <engine/utils/RenderTypes.hpp>

#include <component/CSkyboxCache.hpp>

namespace engine
{
  struct CEnvironment;
}

namespace default_rendering
{

  class EnvironmentSystem : public engine::System
  {
  public:
    EnvironmentSystem() = default;
    ~EnvironmentSystem() override = default;

    void update(float deltaTime) override;

  private:
    CSkyboxCache createSkyboxCache();

  private:
    glm::vec3 m_lastClearColor = glm::vec3(-1.0f);
    engine::CEnvironment *m_environment = nullptr;
  };

} // namespace rasterization_bundle