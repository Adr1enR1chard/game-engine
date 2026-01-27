#include <system/LightSystem.hpp>

#include <component/CCamera.hpp>
#include <component/CDirectionalLight.hpp>
#include <component/CMeshRenderer.hpp>
#include <component/CPointLight.hpp>
#include <component/CTransform.hpp>
#include <service/resource/MaterialResource.hpp>

void LightSystem::update(double /*deltaTime*/)
{
    const auto &[eCamera, cCamera, cCameraTransform] = world().fetchAt<CCamera, CTransform>(0);

    if (!eCamera)
        return;

    const auto &[eDirLight, cDirLight] = world().fetchAt<CDirectionalLight>(0);

    const auto &pointLights = world().fetch<CPointLight, CTransform>();

    MaterialResource *materialResource = services().get<MaterialResource>();
    for (const auto &[eMeshRenderer, cMeshRenderer] : world().fetch<CMeshRenderer>())
    {
        auto &materialRef = cMeshRenderer->materialRef;
        materialResource->setUniform(materialRef, "viewPos", cCameraTransform->position);

        if (cDirLight)
        {
            materialResource->setUniform(materialRef, "dirLight",
                                         Uniform::DirectionalLight{
                                             cDirLight->direction, // direction
                                             cDirLight->color,     // color
                                             cDirLight->ambient,   // ambient
                                             cDirLight->intensity, // intensity
                                         });
        }

        materialResource->setUniform(materialRef, "pointLightCount", static_cast<int>(pointLights.size()));
        int i = 0;
        for (const auto &[ePointLight, cPointLight, cPointLightTransform] : pointLights)
        {
            std::string baseName = "pointLights[" + std::to_string(i) + "]";

            materialResource->setUniform(materialRef, baseName.c_str(),
                                         Uniform::PointLight{
                                             cPointLightTransform->position, // position
                                             cPointLight->color,             // color
                                             cPointLight->intensity,         // intensity
                                         });
            ++i;
        }
    }
}
