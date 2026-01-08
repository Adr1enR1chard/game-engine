#include <engine/system/LightSystem.hpp>

#include <engine/EngineContext.hpp>
#include <engine/component/CTransform.hpp>
#include <engine/component/CDirectionalLight.hpp>
#include <engine/component/CMeshRenderer.hpp>
#include <engine/component/CCamera.hpp>

void LightSystem::update(EngineContext &engineContext, double /*deltaTime*/)
{
    auto &camera = engineContext.currentScene().registry().getEntitiesWithComponents<CCamera, CTransform>();

    if (camera.empty())
        return;

    auto &cameraTransform = engineContext.currentScene().registry().getComponent<CTransform>(camera[0]);

    auto &dirLightQuery = engineContext.currentScene().registry().getEntitiesWithComponent<CDirectionalLight>();

    // If no directional lights, nothing to do for now
    if (dirLightQuery.empty())
        return;

    auto &dirLight = engineContext.currentScene().registry().getComponent<CDirectionalLight>(dirLightQuery[0]);

    auto &renderers = engineContext.currentScene().registry().getEntitiesWithComponents<CMeshRenderer>();

    for (const Entity &entity : renderers)
    {
        auto &meshRenderer = engineContext.currentScene().registry().getComponent<CMeshRenderer>(entity);
        auto &material = meshRenderer.getMaterial();
        auto &shader = material.getShader();

        shader.setDirectionalLight(
            dirLight.direction, // direction
            dirLight.color,     // color
            dirLight.intensity, // intensity
            dirLight.ambient    // ambient
        );
        shader.setVec3("viewPos", cameraTransform.getPosition());
    }
}