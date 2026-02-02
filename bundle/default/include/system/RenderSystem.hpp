#pragma once

#include <glm/glm.hpp>

#include <engine/model/System.hpp>
#include <engine/utils/RenderTypes.hpp>

namespace default_bundle
{

  using namespace engine;

  class RenderSystem : public System
  {
  public:
    void start() override;
    void render(float deltaTime) override;

  private:
    bool m_cameraNotFoundLogged = false;
    ShaderRef m_debugShadowMapShader = 0;
    MeshRef m_debugScreenQuadMesh = 0;
    UniformCollection m_debugShadowMapUniforms;
  };

} // namespace default_bundle