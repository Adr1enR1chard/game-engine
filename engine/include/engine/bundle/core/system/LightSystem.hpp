#pragma once

#include <engine/bundle/core/component/CCamera.hpp>
#include <engine/bundle/core/component/CDirectionalLight.hpp>
#include <engine/bundle/core/component/CMesh.hpp>
#include <engine/bundle/core/component/CPointLight.hpp>
#include <engine/bundle/core/component/CTransform.hpp>
#include <engine/model/System.hpp>

namespace engine
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

} // namespace engine
