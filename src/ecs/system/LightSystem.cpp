#include <engine/system/LightSystem.hpp>

#include <engine/World.hpp>
#include <engine/component/CCamera.hpp>
#include <engine/component/CDirectionalLight.hpp>
#include <engine/component/CMeshRenderer.hpp>
#include <engine/component/CPointLight.hpp>
#include <engine/component/CTransform.hpp>

void LightSystem::update(World& world, double /*deltaTime*/)
{
    auto& camera = world.getEntitiesWithComponents<CCamera, CTransform>();

    if (camera.empty())
        return;

    auto& cameraTransform = world.getComponent<CTransform>(camera[0]);

    auto& dirLights = world.getEntitiesWithComponent<CDirectionalLight>();
    auto& dirLight  = world.getComponent<CDirectionalLight>(dirLights[0]);

    auto& pointLights = world.getEntitiesWithComponents<CPointLight, CTransform>();

    auto& renderers = world.getEntitiesWithComponents<CMeshRenderer>();
    for (const Entity& entity : renderers) {
        auto& meshRenderer = world.getComponent<CMeshRenderer>(entity);
        auto& material     = meshRenderer.material;
        auto& shader       = material->getShader();
        material->setUniform("viewPos", cameraTransform.position);

        shader.setDirectionalLight(dirLight.direction, // direction
                                   dirLight.color,     // color
                                   dirLight.intensity, // intensity
                                   dirLight.ambient    // ambient
        );

        material->setUniform("pointLightCount", static_cast<int>(pointLights.size()));
        for (size_t i = 0; i < pointLights.size(); ++i) {
            auto& pointLight     = world.getComponent<CPointLight>(pointLights[i]);
            auto& lightTransform = world.getComponent<CTransform>(pointLights[i]);

            std::string baseName = "pointLights[" + std::to_string(i) + "]";

            material->setUniform(baseName + ".position", lightTransform.position);
            material->setUniform(baseName + ".color", pointLight.color);
            material->setUniform(baseName + ".intensity", pointLight.intensity);
            material->setUniform(baseName + ".radius", pointLight.radius);
        }
    }
}