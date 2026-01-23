#include <system/LightSystem.hpp>

#include <component/CCamera.hpp>
#include <component/CDirectionalLight.hpp>
#include <component/CMeshRenderer.hpp>
#include <component/CPointLight.hpp>
#include <component/CTransform.hpp>

void LightSystem::update(double /*deltaTime*/)
{

    const auto& [eCamera, cCamera, cCameraTransform] = world().getAt<CCamera, CTransform>(0);

    if (!eCamera)
        return;

    const auto& [eDirLight, cDirLight] = world().getAt<CDirectionalLight>(0);

    const auto& pointLights = world().get<CPointLight, CTransform>();

    // for (const auto& [eMeshRenderer, cMeshRenderer] : world().get<CMeshRenderer>()) {
    //     auto& materialInstance = cMeshRenderer->materialRef;
    //     materialInstance.setUniform("viewPos", cCameraTransform->position);

    //     if (cDirLight) {
    //         materialInstance.setUniform("dirLight", Uniform::DirectionalLight{
    //                                                     cDirLight->direction, // direction
    //                                                     cDirLight->color,     // color
    //                                                     cDirLight->ambient,   // ambient
    //                                                     cDirLight->intensity, // intensity
    //                                                 });
    //     }

    //     materialInstance.setUniform("pointLightCount", static_cast<int>(pointLights.size()));
    //     int i = 0;
    //     for (const auto& [ePointLight, cPointLight, cPointLightTransform] : pointLights) {
    //         std::string baseName = "pointLights[" + std::to_string(i) + "]";

    //         materialInstance.setUniform(baseName, Uniform::PointLight{
    //                                                   cPointLightTransform->position, // position
    //                                                   cPointLight->color,             // color
    //                                                   cPointLight->intensity,         // intensity
    //                                               });
    //         ++i;
    //     }
    // }
}
