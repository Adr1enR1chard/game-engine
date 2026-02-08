#include <service/ShadowMapping.hpp>

#include <glad/glad.h>
#include <engine/utils/Log.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace default_rendering
{

    using namespace engine;

    ShadowMapping::ShadowMapping(Renderer &renderer, ShaderFactory &shaderFactory)
        : m_renderer(renderer), m_shaderFactory(shaderFactory), m_width(1024), m_height(1024), m_depthFramebuffer(0)
    {
        float extent = 20.0f;
        m_lightProjectionMatrix = glm::ortho(-extent, extent, -extent, extent, m_nearPlane, m_farPlane);
    }
    ShadowMapping::~ShadowMapping() = default;

    FramebufferRef ShadowMapping::initialize()
    {
        m_depthShader = m_shaderFactory.CustomShader("__ShadowMappingDepthShader",
                                                     "default-bundle-assets/shaders/shadow_mapping/depth.vert",
                                                     "default-bundle-assets/shaders/shadow_mapping/depth.frag",
                                                     {},
                                                     {
                                                         .enableBackfaceCulling = false,
                                                         .enableDepthTest = true,
                                                         .enableDepthWrite = true,
                                                     });
        createDepthBuffer();
        return m_depthFramebuffer;
    }

    void ShadowMapping::createDepthBuffer()
    {
        m_depthFramebuffer = m_renderer.allocateFramebuffer(m_width, m_height, false, true, false, false, false);
        m_depthTexture = m_renderer.getFramebufferDepthAttachment(m_depthFramebuffer);

        m_renderer.setTextureBorderColor(m_depthTexture, glm::vec4(1.0f));
        m_renderer.setTextureWrapMode(m_depthTexture, Renderer::TextureWrapMode::ClampToBorder, Renderer::TextureWrapMode::ClampToBorder, Renderer::TextureWrapMode::ClampToBorder);
        m_renderer.setTextureFilterMode(m_depthTexture, Renderer::TextureFilterMode::Nearest, Renderer::TextureFilterMode::Nearest);
    }

    FramebufferRef ShadowMapping::getDepthBuffer() const
    {
        if (!m_enabled)
            return 0;
        return m_depthFramebuffer;
    }

    glm::mat4 ShadowMapping::getLightSpaceMatrix(glm::vec3 lightDir, glm::vec3 target) const
    {
        return m_lightProjectionMatrix * glm::lookAt(target - lightDir * 5.0f, target, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    // void ShadowMapping::restoreAfterRender()
    // {
    //     glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //     glViewport(m_viewportBackup[0], m_viewportBackup[1], m_viewportBackup[2], m_viewportBackup[3]);
    // }

    void ShadowMapping::setShadowMapDimensions(unsigned int width, unsigned int height)
    {
        m_width = width;
        m_height = height;

        if (m_depthFramebuffer != 0)
        {
            m_renderer.freeFramebuffer(m_depthFramebuffer);
            createDepthBuffer();
        }
    }

} // namespace engine