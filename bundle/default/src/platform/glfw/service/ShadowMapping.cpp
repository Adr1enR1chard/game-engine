#include <service/ShadowMapping.hpp>

#include <glad/glad.h>
#include <engine/utils/Log.hpp>

namespace default_bundle
{

    using namespace engine;

    ShadowMapping::ShadowMapping(TextureResource &textureResource, ShaderFactory &shaderFactory)
        : m_textureResource(textureResource), m_shaderFactory(shaderFactory), m_width(1024), m_height(1024), m_depthMap(0)
    {
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
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Log::Print("Failed to create framebuffer for shadow mapping.", LogLevel::Error);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_framebuffer = std::unique_ptr<FramebufferImpl, FramebufferDeleter>(new FramebufferImpl{fbo});

        return m_depthMap;
    }

    TextureRef ShadowMapping::getDepthMap() const
    {
        return m_depthMap;
    }

    void ShadowMapping::renderDepth(std::function<void(ShaderRef shaderRef, TextureRef depthMap, glm::mat4 lightSpaceMatrix)> renderScene, glm::vec3 lightDir)
    {
        if (m_depthMap == 0)
        {
            Log::Print("Depth map not created for shadow mapping.", LogLevel::Error);
            return;
        }

        // Bind the depth map framebuffer
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glViewport(0, 0, m_width, m_height); // Assuming a fixed size for simplicity
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer->fbo);
        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 100.0f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(-lightDir * 5.0f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;

        // Render the scene to populate the depth map
        renderScene(m_depthShader, m_depthMap, lightSpaceMatrix);

        // Unbind the framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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