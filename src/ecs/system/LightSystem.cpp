#include <engine/system/LightSystem.hpp>

#include <engine/EngineContext.hpp>
#include <engine/component/CTransform.hpp>
#include <engine/component/CDirectionalLight.hpp>
#include <engine/component/CMeshRenderer.hpp>
#include <engine/component/CCamera.hpp>
#include <engine/component/CPointLight.hpp>

void LightSystem::update(EngineContext &engineContext, double /*deltaTime*/)
{
    auto &camera = engineContext.currentScene().registry().getEntitiesWithComponents<CCamera, CTransform>();

    if (camera.empty())
        return;

    auto &cameraTransform = engineContext.currentScene().registry().getComponent<CTransform>(camera[0]);

    auto &dirLights = engineContext.currentScene().registry().getEntitiesWithComponent<CDirectionalLight>();
    auto &dirLight = engineContext.currentScene().registry().getComponent<CDirectionalLight>(dirLights[0]);

    auto &pointLights = engineContext.currentScene().registry().getEntitiesWithComponents<CPointLight, CTransform>();

    auto &renderers = engineContext.currentScene().registry().getEntitiesWithComponents<CMeshRenderer>();

    for (const Entity &entity : renderers)
    {
        auto &meshRenderer = engineContext.currentScene().registry().getComponent<CMeshRenderer>(entity);
        auto &material = meshRenderer.getMaterial();
        auto &shader = material.getShader();
        material.setUniform("viewPos", cameraTransform.getPosition());

        shader.setDirectionalLight(
            dirLight.direction, // direction
            dirLight.color,     // color
            dirLight.intensity, // intensity
            dirLight.ambient    // ambient
        );

        material.setUniform("pointLightCount", static_cast<int>(pointLights.size()));
        for (size_t i = 0; i < pointLights.size(); ++i)
        {
            auto &pointLight = engineContext.currentScene().registry().getComponent<CPointLight>(pointLights[i]);
            auto &lightTransform = engineContext.currentScene().registry().getComponent<CTransform>(pointLights[i]);

            std::string baseName = "pointLights[" + std::to_string(i) + "]";

            material.setUniform(baseName + ".position", lightTransform.getPosition());
            material.setUniform(baseName + ".color", pointLight.color);
            material.setUniform(baseName + ".intensity", pointLight.intensity);
            material.setUniform(baseName + ".radius", pointLight.radius);
        }
    }
}