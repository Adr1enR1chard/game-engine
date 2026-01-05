#include "RenderSystem.hpp"

#include <stdio.h>
#include <string>
#include <glad/glad.h>

#include "scene/Scene.hpp"

#include "ecs/component/CMeshRenderer.hpp"
#include "ecs/component/CTransformCache.hpp"

#include "handle/ShaderHandle.hpp"
#include "handle/TextureHandle.hpp"

void RenderSystem::update(Scene &scene, float deltaTime)
{
    Registry &registry = scene.getRegistry();

    for (const Entity &entity : registry.getEntitiesWithComponents<CMeshRenderer, CTransformCache>())
    {
        auto &transform = registry.getComponent<CTransformCache>(entity);
        auto &meshRenderer = registry.getComponent<CMeshRenderer>(entity);

        if (!meshRenderer.material || !meshRenderer.mesh)
        {
            continue;
        }

        const ShaderHandle &shader = meshRenderer.material->getShader();
        shader.use();
        shader.setMat4("view", scene.getMainCamera().getViewMatrix());
        shader.setMat4("projection", scene.getMainCamera().getProjectionMatrix());
        shader.setMat4("model", transform.modelMatrix);

        // Bind textures to consecutive units and set sampler uniforms to those units
        const auto &textures = meshRenderer.material->getTextures();
        const auto &texNames = meshRenderer.material->getTextureNames();
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

        if (meshRenderer.mesh)
        {
            glBindVertexArray(meshRenderer.mesh->VAO);
            glDrawArrays(GL_TRIANGLES, 0, meshRenderer.mesh->getVertexCount());
        }
    }

    deltaTime;
}
