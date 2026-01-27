#pragma once

#include <component/CCamera.hpp>
#include <component/CDirectionalLight.hpp>
#include <component/CMeshRenderer.hpp>
#include <component/CPointLight.hpp>
#include <component/CTransform.hpp>
#include <engine/model/System.hpp>

class LightSystem : public System
{
public:
  void update(double deltaTime) override;

private:
  void setMaterialLights(MaterialRef materialRef, const CCamera *camera,
                         const CTransform *cameraTransform,
                         const CDirectionalLight *dirLight,
                         const std::vector<std::tuple<Entity, CPointLight *, CTransform *>> &pointLights);
};
