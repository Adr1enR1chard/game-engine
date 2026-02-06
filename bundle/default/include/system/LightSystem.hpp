#pragma once

#include <component/CCamera.hpp>
#include <component/CDirectionalLight.hpp>
#include <component/CMeshRenderer.hpp>
#include <component/CPointLight.hpp>
#include <component/CTransform.hpp>
#include <engine/model/System.hpp>

namespace default_bundle
{

  class LightSystem : public engine::System
  {
  public:
    void update(float deltaTime) override;

  private:
    void setMaterialLights(MaterialHandle &material,
                           const CTransform *cameraTransform,
                           const CDirectionalLight *dirLight,
                           const std::vector<std::tuple<engine::Entity, CPointLight *, CTransform *>> &pointLights);
  };

} // namespace default_bundle
