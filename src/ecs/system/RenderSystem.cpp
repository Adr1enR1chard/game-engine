#include <engine/system/RenderSystem.hpp>

#include <stdio.h>
#include <string>
#include <glad/glad.h>

#include <engine/EngineContext.hpp>
#include <engine/SceneManager.hpp>

#include <engine/component/CMeshRenderer.hpp>
#include <engine/component/CTransformCache.hpp>
#include <engine/component/CCamera.hpp>
#include <engine/component/CCameraCache.hpp>

#include "handle/ShaderHandle.hpp"
#include "handle/TextureHandle.hpp"

#include <engine/Time.hpp>
#include <scene/Scene.hpp>

void RenderSystem::update(EngineContext &engineContext)
{
    Registry &registry = engineContext.registry();
    auto cameraEntities = registry.getEntitiesWithComponents<CTransformCache, CCameraCache>();
    if (cameraEntities.empty())
        return;

    const auto &cameraCache = registry.getComponent<CCameraCache>(cameraEntities[0]);
    const auto &cameraTransform = registry.getComponent<CTransformCache>(cameraEntities[0]);

    for (const Entity &entity : registry.getEntitiesWithComponents<CMeshRenderer, CTransformCache>())
    {
        auto &transform = registry.getComponent<CTransformCache>(entity);
        auto &meshRenderer = registry.getComponent<CMeshRenderer>(entity);

        auto &material = meshRenderer.getMaterial();
        auto &mesh = meshRenderer.getMesh();
        auto &shader = material.getShader();

        shader.use();
        shader.setMat4("view", cameraTransform.viewMatrix);
        shader.setMat4("model", transform.modelMatrix);
        shader.setMat4("projection", cameraCache.projectionMatrix);

        // Bind textures to consecutive units and set sampler uniforms to those units
        const auto &textures = material.getTextures();
        const auto &texNames = material.getTextureNames();
        for (size_t i = 0; i < textures.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(i));
            textures[i].bind();

            // Use provided name or default to "texture{i}" if names list is shorter
            if (i < texNames.size())
            {
                shader.setInt(texNames[i].c_str(), static_cast<int>(i));
            }
            else
            {
                std::string defaultName = "texture" + std::to_string(i);
                shader.setInt(defaultName.c_str(), static_cast<int>(i));
            }
        }

        glBindVertexArray(mesh.VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());
    }
}
