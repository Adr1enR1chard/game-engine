#pragma once

#include <glm/glm.hpp>

#include <engine/model/System.hpp>
#include <engine/utils/RenderTypes.hpp>

namespace engine
{
  class ShaderFactory;
  class MeshFactory;
}

namespace default_rendering
{

  using namespace engine;

  class RenderSystem : public System
  {
  public:
    void start() override;
    void update(float deltaTime) override;

  private:
    void setFinalRenderingUniforms(UniformCollection &uniforms, const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix, const glm::mat4 &projMatrix, const glm::mat4 &lightSpaceMatrix, Sampler2D shadowMap, float bias);

    bool m_cameraNotFoundLogged = false;

    void initializeShadowMapVisualizer(class ShadowMapping *shadowMapping);
    void initializeImageShader(class ShaderFactory *shaderFactory, class MeshFactory *meshFactory);

    TextureRef m_whiteTexture = 0;
    FramebufferRef m_gBuffer = 0;

    ShaderRef m_imageShader = 0;
    MeshRef m_imageQuadMesh = 0;
    // Debugging
    UniformCollection m_debugShadowMapUniforms;
  };

} // namespace rasterization_bundle