#pragma once
#include <glm/glm.hpp>

#include <engine/model/System.hpp>
#include <engine/model/Entity.hpp>
#include <component/CEnvironment.hpp>
#include <component/cache/CSkyboxCache.hpp>
#include <engine/utils/RenderTypes.hpp>

namespace default_bundle
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
    CEnvironment *m_environment = nullptr;
  };

} // namespace default_bundle