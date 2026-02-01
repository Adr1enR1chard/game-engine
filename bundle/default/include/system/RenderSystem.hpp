#pragma once

#include <glm/glm.hpp>

#include <engine/model/System.hpp>

namespace default_bundle
{

  class RenderSystem : public engine::System
  {
  public:
    void start() override;
    void render(float deltaTime) override;

  private:
    bool m_cameraNotFoundLogged = false;
  };

} // namespace default_bundle