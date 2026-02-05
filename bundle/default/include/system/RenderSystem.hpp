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
    void setFinalRenderingUniforms(UniformCollection &uniforms, const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix, const glm::mat4 &lightSpaceMatrix, FramebufferRef shadowMap, float bias);

    bool m_cameraNotFoundLogged = false;

    void initializeShadowMapVisualizer(class ShadowMapping *shadowMapping, class ShaderFactory *shaderFactory, class MeshFactory *meshFactory);
    ShaderRef m_debugShadowMapShader = 0;
    MeshRef m_debugScreenQuadMesh = 0;
    UniformCollection m_debugShadowMapUniforms;
    TextureRef m_whiteTexture = 0;
  };

} // namespace default_bundle