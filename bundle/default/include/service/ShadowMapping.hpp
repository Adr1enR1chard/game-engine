#pragma once

#include <engine/model/Service.hpp>
#include <engine/service/resource/TextureResource.hpp>
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
        ShadowMapping(TextureResource &textureResource, ShaderFactory &shaderFactory);
        ~ShadowMapping() override;

        void setDimensions(unsigned int width, unsigned int height);
        // void getDimensions(unsigned int &outWidth, unsigned int &outHeight) const;
        void setBias(float bias) { m_bias = bias; }
        float getBias() const { return m_bias; }

    private:
        friend class RenderSystem;

        float getNearPlane() const { return m_nearPlane; }
        float getFarPlane() const { return m_farPlane; }

        TextureRef createDepthMap();
        TextureRef getDepthMap() const;
        ShaderRef getDepthShader() const { return m_depthShader; }

        glm::mat4 getLightSpaceMatrix() const { return m_lightProjectionMatrix * m_lightViewMatrix; }

        void prepareForRender(ShaderResource *shaderResource, glm::vec3 lightDir, glm::vec3 target);
        void restoreAfterRender();

    private:
        TextureResource &m_textureResource;
        ShaderFactory &m_shaderFactory;

        unsigned int m_width = 1024;
        unsigned int m_height = 1024;
        float m_bias = 0.005f;
        float m_nearPlane = 0.1f;
        float m_farPlane = 100.0f;

        TextureRef m_depthMap;
        glm::mat4 m_lightProjectionMatrix;
        glm::mat4 m_lightViewMatrix;

        struct FramebufferImpl;
        struct FramebufferDeleter
        {
            void operator()(FramebufferImpl *framebufferImpl);
        };
        std::unique_ptr<FramebufferImpl, FramebufferDeleter> m_framebuffer;

        ShaderRef m_depthShader;

        int m_viewportBackup[4];
    };

} // namespace default_bundle