#pragma once

#include <glm/glm.hpp>

#include <engine/model/System.hpp>

class RenderSystem : public System
{
public:
  void render(float deltaTime) override;

private:
  bool m_cameraNotFoundLogged = false;
};