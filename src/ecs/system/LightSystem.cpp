#include <engine/system/LightSystem.hpp>

#include <engine/World.hpp>
#include <engine/component/CCamera.hpp>
#include <engine/component/CDirectionalLight.hpp>
#include <engine/component/CMeshRenderer.hpp>
#include <engine/component/CPointLight.hpp>
#include <engine/component/CTransform.hpp>

void LightSystem::update(World& world, double /*deltaTime*/)
{

    const auto& [eCamera, cCamera, cCameraTransform] = world.getAt<CCamera, CTransform>(0);

    if (!eCamera)
        return;

    const auto& [eDirLight, cDirLight] = world.getAt<CDirectionalLight>(0);

    const auto& pointLights = world.get<CPointLight, CTransform>();

    for (const auto& [eMeshRenderer, cMeshRenderer] : world.get<CMeshRenderer>()) {
        auto& materialInstance = cMeshRenderer->material;
        materialInstance.setUniform("viewPos", cCameraTransform->position);

        if (cDirLight) {
            materialInstance.setUniform("dirLight", UDirectionalLight{
                                                        cDirLight->direction, // direction
                                                        cDirLight->color,     // color
                                                        cDirLight->intensity, // intensity
                                                        cDirLight->ambient    // ambient
                                                    });
        }

        materialInstance.setUniform("pointLightCount", static_cast<int>(pointLights.size()));
        int i = 0;
        for (const auto& [ePointLight, cPointLight, cPointLightTransform] : pointLights) {
            std::string baseName = "pointLights[" + std::to_string(i) + "]";

            materialInstance.setUniform(baseName, UPointLight{
                                                      cPointLightTransform->position, // position
                                                      cPointLight->color,             // color
                                                      cPointLight->intensity,         // intensity
                                                      cPointLight->radius             // radius
                                                  });
        }
    }
}
