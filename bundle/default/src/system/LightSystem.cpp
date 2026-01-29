#include <system/LightSystem.hpp>

#include <service/resource/MaterialResource.hpp>
#include <service/resource/ModelResource.hpp>

void LightSystem::update(double /*deltaTime*/)
{
    const auto &[eCamera, cCamera, cCameraTransform] = world().fetchAt<CCamera, CTransform>(0);

    if (!eCamera)
        return;

    const auto &[eDirLight, cDirLight] = world().fetchAt<CDirectionalLight>(0);

    const auto &pointLights = world().fetch<CPointLight, CTransform>();

    // Mesh renderers
    for (const auto &[eMeshRenderer, cMeshRenderer] : world().fetch<CMeshRenderer>())
    {
        setMaterialLights(cMeshRenderer->materialRef, cCameraTransform, cDirLight, pointLights);
    }

    // Model renderers
    ModelResource *modelResource = services().get<ModelResource>();
    for (const auto &[eModelRenderer, cModelRenderer] : world().fetch<CModelRenderer>())
    {
        modelResource->forEach(cModelRenderer->modelRef,
                               [&](MeshRef /*meshRef*/, MaterialRef materialRef, size_t /*index*/)
                               {
                                   setMaterialLights(materialRef, cCameraTransform, cDirLight, pointLights);
                               });
    }
}

void LightSystem::setMaterialLights(MaterialRef materialRef,
                                    const CTransform *cameraTransform,
                                    const CDirectionalLight *dirLight,
                                    const std::vector<std::tuple<Entity, CPointLight *, CTransform *>> &pointLights)
{
    MaterialResource *materialResource = services().get<MaterialResource>();
    materialResource->setUniform(materialRef, "viewPos", cameraTransform->position);

    if (dirLight)
    {
        materialResource->setUniform(materialRef, "dirLight",
                                     Uniform::DirectionalLight{
                                         dirLight->direction, // direction
                                         dirLight->color,     // color
                                         dirLight->ambient,   // ambient
                                         dirLight->intensity, // intensity
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