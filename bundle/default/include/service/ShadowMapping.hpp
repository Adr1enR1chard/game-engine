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

        TextureRef createDepthMap();
        TextureRef getDepthMap() const;

        void renderDepth(std::function<void(ShaderRef shaderRef, TextureRef depthMap, glm::mat4 lightSpaceMatrix)> renderScene, glm::vec3 lightDir);

        void setDimensions(unsigned int width, unsigned int height);

    private:
        TextureResource &m_textureResource;
        ShaderFactory &m_shaderFactory;

        TextureRef m_depthMap;
        unsigned int m_width;
        unsigned int m_height;

        struct FramebufferImpl;
        struct FramebufferDeleter
        {
            void operator()(FramebufferImpl *framebufferImpl);
        };
        std::unique_ptr<FramebufferImpl, FramebufferDeleter> m_framebuffer;

        ShaderRef m_depthShader;
    };

} // namespace default_bundle