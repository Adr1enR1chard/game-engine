#pragma once

#include <engine/registry/World.hpp>
#include <engine/registry/ServiceRegistry.hpp>
#include <string>
#include <functional>

using namespace engine;

namespace engine
{
    struct CMesh;
}

class InspectorPanel
{
public:
    InspectorPanel(World &world, ServiceRegistry &services) : world(world), services(services) {}
    ~InspectorPanel() = default;

    void draw(Entity selectedEntity);

private:
    World &world;
    ServiceRegistry &services;
    char m_nameBuffer[256] = "";
    char m_pathBuffer[256] = "";
    Entity m_lastSelectedEntity = 0;

    // Component-specific drawing methods
    void drawTransformComponent(Entity entity);
    void drawCameraComponent(Entity entity);
    void drawDirectionalLightComponent(Entity entity);
    void drawPointLightComponent(Entity entity);
    void drawMeshComponent(Entity entity);
    void drawModelComponent(Entity entity);
    void drawEnvironmentComponent(Entity entity);

    void setMaterialTexture(engine::CMesh *mesh, const std::string &uniformName, const std::string &path);

    // Helper methods
    void drawVec3Control(const char *label, glm::vec3 &values, float resetValue = 0.0f, float columnWidth = 100.0f, float min = 0.0f, float max = 0.0f);
    void drawFloatControl(const char *label, float &value, float speed = 0.1f, float min = 0.0f, float max = 0.0f);
    void drawLoadResourcePopup(const char *label, std::function<void(const std::string &)> onLoad);
};