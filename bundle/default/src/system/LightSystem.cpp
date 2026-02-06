#include <system/LightSystem.hpp>

#include <engine/bundle/standalone/service/Renderer.hpp>

namespace default_bundle
{
    using namespace engine;

    void LightSystem::update(float /*deltaTime*/)
    {
        const auto &[eCamera, cCamera, cCameraTransform] = world().fetchAt<CCamera, CTransform>(0);

        if (!eCamera)
            return;

        const auto &[eDirLight, cDirLight] = world().fetchAt<CDirectionalLight>(0);

        const auto &pointLights = world().fetch<CPointLight, CTransform>();

        // Mesh renderers
        for (const auto &[eMeshRenderer, cMeshRenderer] : world().fetch<CMeshRenderer>())
        {
            setMaterialLights(cMeshRenderer->material, cCameraTransform, cDirLight, pointLights);
        }

        // Model renderers
        Renderer *renderer = services().get<Renderer>();
        for (const auto &[eModelRenderer, cModelRenderer] : world().fetch<CModelRenderer>())
        {
            cModelRenderer->model.forEach([&](MeshRef /*meshRef*/, MaterialHandle &material, size_t /*index*/)
                                          { setMaterialLights(material, cCameraTransform, cDirLight, pointLights); });
        }
    }

    void LightSystem::setMaterialLights(MaterialHandle &material,
                                        const CTransform *cameraTransform,
                                        const CDirectionalLight *dirLight,
                                        const std::vector<std::tuple<Entity, CPointLight *, CTransform *>> &pointLights)
    {
        material.uniforms["viewPos"] = cameraTransform->position;

        if (dirLight)
        {
            material.uniforms["dirLight.direction"] = dirLight->direction;
            material.uniforms["dirLight.color"] = dirLight->color;
            material.uniforms["dirLight.ambient"] = dirLight->ambient;
            material.uniforms["dirLight.intensity"] = dirLight->intensity;
        }

        material.uniforms["pointLightCount"] = static_cast<int>(pointLights.size());
        int i = 0;
        for (const auto &[ePointLight, cPointLight, cPointLightTransform] : pointLights)
        {
            std::string baseName = "pointLights[" + std::to_string(i) + "]";

            material.uniforms[baseName + ".position"] = cPointLightTransform->position;
            material.uniforms[baseName + ".color"] = cPointLight->color;
            material.uniforms[baseName + ".intensity"] = cPointLight->intensity;
            ++i;
        }
    }
} // namespace default_bundle
