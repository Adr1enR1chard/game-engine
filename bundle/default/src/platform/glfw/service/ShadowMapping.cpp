#include <service/ShadowMapping.hpp>

#include <glad/glad.h>
#include <engine/utils/Log.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace default_bundle
{

    using namespace engine;

    ShadowMapping::ShadowMapping(TextureResource &textureResource, ShaderFactory &shaderFactory)
        : m_textureResource(textureResource), m_shaderFactory(shaderFactory), m_width(4096), m_height(4096), m_depthMap(0)
    {
        float extent = 10.0f;
        m_lightProjectionMatrix = glm::ortho(-extent, extent, -extent, extent, m_nearPlane, m_farPlane);
    }
    ShadowMapping::~ShadowMapping() = default;

    struct ShadowMapping::FramebufferImpl
    {
        unsigned int fbo;
    };

    void ShadowMapping::FramebufferDeleter::operator()(FramebufferImpl *framebufferImpl)
    {
        if (framebufferImpl)
        {
            glDeleteFramebuffers(1, &framebufferImpl->fbo);
            delete framebufferImpl;
        }
    }

    TextureRef ShadowMapping::createDepthMap()
    {
        m_depthShader = m_shaderFactory.CustomShader("__ShadowMappingDepthShader",
                                                     "default-bundle-assets/shaders/shadow_mapping/depth.vert",
                                                     "default-bundle-assets/shaders/shadow_mapping/depth.frag", {});

        unsigned int fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Create depth map texture and attach it to the framebuffer
        m_depthMap = m_textureResource.depthMap(m_width, m_height);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Log::Print("Failed to create framebuffer for shadow mapping.", LogLevel::Error);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        Log::Print("Created framebuffer for shadow mapping with FBO ID: " + std::to_string(fbo), LogLevel::Debug);
        m_framebuffer = std::unique_ptr<FramebufferImpl, FramebufferDeleter>(new FramebufferImpl{fbo});

        return m_depthMap;
    }

    TextureRef ShadowMapping::getDepthMap() const
    {
        return m_depthMap;
    }

    void ShadowMapping::prepareForRender(ShaderResource *shaderResource, glm::vec3 lightDir, glm::vec3 target)
    {
        shaderResource->bind(m_depthShader, glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f));

        glGetIntegerv(GL_VIEWPORT, m_viewportBackup);
        glViewport(0, 0, m_width, m_height); // Assuming a fixed size for simplicity
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer->fbo);
        glClear(GL_DEPTH_BUFFER_BIT);

        m_lightViewMatrix = glm::lookAt(target - lightDir * 5.0f, target, glm::vec3(0.0f, 1.0f, 0.0f));

        shaderResource->setUniform(m_depthShader, "uLightSpaceMatrix", getLightSpaceMatrix());
    }

    void ShadowMapping::restoreAfterRender()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(m_viewportBackup[0], m_viewportBackup[1], m_viewportBackup[2], m_viewportBackup[3]);
    }

    void ShadowMapping::setDimensions(unsigned int width, unsigned int height)
    {
        m_width = width;
        m_height = height;

        if (m_depthMap != 0)
        {
            m_textureResource.remove(m_depthMap);
            m_framebuffer.reset();
            createDepthMap();
        }
    }

} // namespace default_bundle