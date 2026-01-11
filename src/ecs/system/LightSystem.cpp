#include <engine/system/LightSystem.hpp>

#include <engine/World.hpp>
#include <engine/component/CCamera.hpp>
#include <engine/component/CDirectionalLight.hpp>
#include <engine/component/CMeshRenderer.hpp>
#include <engine/component/CPointLight.hpp>
#include <engine/component/CTransform.hpp>

void LightSystem::update(World& world, double /*deltaTime*/)
{

    const auto& [cameraEntity, cameraComponent, cameraTransform] = world.getAt<CCamera, CTransform>(0);

    if (!cameraEntity)
        return;

    const auto& [dirLightEntity, dirLight] = world.getAt<CDirectionalLight>(0);

    const auto& pointLights = world.get<CPointLight, CTransform>();

    for (const auto& [entity, meshRenderer] : world.get<CMeshRenderer>()) {
        auto& material = meshRenderer->material;
        auto& shader   = material->getShader();
        material->setUniform("viewPos", cameraTransform->position);

        if (dirLight) {
            shader.setDirectionalLight(dirLight->direction, // direction
                                       dirLight->color,     // color
                                       dirLight->intensity, // intensity
                                       dirLight->ambient    // ambient
            );
        }

        material->setUniform("pointLightCount", static_cast<int>(pointLights.size()));
        int i = 0;
        for (const auto& [lightEntity, pointLight, lightTransform] : pointLights) {
            std::string baseName = "pointLights[" + std::to_string(i) + "]";

            material->setUniform(baseName + ".position", lightTransform->position);
            material->setUniform(baseName + ".color", pointLight->color);
            material->setUniform(baseName + ".intensity", pointLight->intensity);
            material->setUniform(baseName + ".radius", pointLight->radius);
            i++;
        }
    }
}
