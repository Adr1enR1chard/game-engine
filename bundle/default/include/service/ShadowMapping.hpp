#pragma once

#include <engine/model/Service.hpp>
#include <engine/bundle/standalone/service/Renderer.hpp>
#include <service/factory/ShaderFactory.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <functional>

namespace default_bundle
{

    using namespace engine;

    class ShadowMapping : public Service
    {
    public:
        ShadowMapping(Renderer &renderer, ShaderFactory &shaderFactory);
        ~ShadowMapping() override;

        void setShadowMapDimensions(unsigned int width, unsigned int height);
        // void getDimensions(unsigned int &outWidth, unsigned int &outHeight) const;
        void setBias(float bias) { m_bias = bias; }
        float getBias() const { return m_bias; }
        void setLightProjection(float nearPlane, float farPlane, float extent)
        {
            m_nearPlane = nearPlane;
            m_farPlane = farPlane;
            m_lightProjectionMatrix = glm::ortho(-extent, extent, -extent, extent, m_nearPlane, m_farPlane);
        }

        bool enabled() const { return m_enabled; }
        void setEnabled(bool enabled)
        {
            m_enabled = enabled;
        }

    private:
        friend class RenderSystem;

        float getNearPlane() const { return m_nearPlane; }
        float getFarPlane() const { return m_farPlane; }

        FramebufferRef initializeDepthBuffer();
        FramebufferRef createDepthBuffer();
        FramebufferRef getDepthBuffer() const;
        ShaderRef getDepthShader() const { return m_depthShader; }

        glm::mat4 getLightSpaceMatrix(glm::vec3 lightDir, glm::vec3 target) const;

    private:
        Renderer &m_renderer;
        ShaderFactory &m_shaderFactory;

        unsigned int m_width = 1024;
        unsigned int m_height = 1024;
        float m_bias = 0.005f;
        float m_nearPlane = -100.0f;
        float m_farPlane = 100.0f;

        FramebufferRef m_depthFramebuffer = 0;
        glm::mat4 m_lightProjectionMatrix;
        glm::mat4 m_lightViewMatrix;

        ShaderRef m_depthShader;

        int m_viewportBackup[4];
        bool m_enabled = true;
    };

} // namespace default_bundle