#include "InspectorPanel.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <DefaultRenderingBundle.hpp>
#include <engine/bundle/core/CoreBundle.hpp>

using namespace default_rendering;
using namespace engine;

void InspectorPanel::draw(Entity selectedEntity)
{
    ImGui::Begin("Inspector");

    if (selectedEntity == 0)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
        ImGui::TextWrapped("No entity selected.\nSelect an entity from the Entities panel to inspect its components.");
        ImGui::PopStyleColor();
        ImGui::End();
        return;
    }

    // Entity header
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
    ImGui::AlignTextToFramePadding();
    ImGui::Text("Entity");
    ImGui::SameLine();

    // Entity ID badge
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.4f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.4f, 0.8f, 1.0f));
    ImGui::Button(std::to_string(selectedEntity).c_str());
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();

    ImGui::Separator();
    ImGui::Spacing();

    // Entity name section
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));

    // Update name buffer when entity changes
    if (m_lastSelectedEntity != selectedEntity)
    {
        snprintf(m_nameBuffer, sizeof(m_nameBuffer), "Entity %d", selectedEntity);
        m_lastSelectedEntity = selectedEntity;
    }

    ImGui::SetNextItemWidth(-1);
    if (ImGui::InputText("##EntityName", m_nameBuffer, sizeof(m_nameBuffer)))
    {
        // Entity name changed - could add a CName component here
    }
    ImGui::PopStyleVar();
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Components section
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.0f);

    drawTransformComponent(selectedEntity);
    drawCameraComponent(selectedEntity);
    drawDirectionalLightComponent(selectedEntity);
    drawPointLightComponent(selectedEntity);
    drawMeshComponent(selectedEntity);
    drawModelComponent(selectedEntity);
    drawEnvironmentComponent(selectedEntity);

    ImGui::PopStyleVar();
    ImGui::Spacing();

    // Add component button
    ImGui::Separator();
    ImGui::Spacing();
    if (ImGui::Button("Add Component", ImVec2(-1, 0)))
    {
        ImGui::OpenPopup("AddComponentPopup");
    }

    if (ImGui::BeginPopup("AddComponentPopup"))
    {
        ImGui::SeparatorText("Add Component");

        if (ImGui::MenuItem("Transform"))
        {
            if (!world.hasComponents<CTransform>(selectedEntity))
                world.addComponents(selectedEntity, CTransform{});
        }
        if (ImGui::MenuItem("Camera"))
        {
            if (!world.hasComponents<CCamera>(selectedEntity))
                world.addComponents(selectedEntity, CCamera{});
        }
        if (ImGui::MenuItem("Directional Light"))
        {
            if (!world.hasComponents<CDirectionalLight>(selectedEntity))
                world.addComponents(selectedEntity, CDirectionalLight{});
        }
        if (ImGui::MenuItem("Point Light"))
        {
            if (!world.hasComponents<CPointLight>(selectedEntity))
                world.addComponents(selectedEntity, CPointLight{});
        }
        if (ImGui::MenuItem("Mesh Renderer"))
        {
            if (!world.hasComponents<CMesh>(selectedEntity))
                world.addComponents(selectedEntity, CMesh{});
        }
        if (ImGui::MenuItem("Environment"))
        {
            if (!world.hasComponents<CEnvironment>(selectedEntity))
                world.addComponents(selectedEntity, CEnvironment{});
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}

void InspectorPanel::drawTransformComponent(Entity entity)
{
    if (!world.hasComponents<CTransform>(entity))
        return;

    auto [e, transform] = world.fetchFrom<CTransform>(entity);

    ImGui::PushID("Transform");
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

    bool open = ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen);

    ImGui::PopStyleVar();

    if (open)
    {
        ImGui::Indent();

        // Position
        drawVec3Control("Position", transform->position);

        // Rotation (as Euler angles)
        glm::vec3 euler = glm::degrees(glm::eulerAngles(transform->rotation));
        drawVec3Control("Rotation", euler);
        transform->setRotationFromEuler(glm::radians(euler));

        // Scale
        glm::vec3 scale = transform->scale;
        drawVec3Control("Scale", scale, 1.0f);
        // Prevent negative or zero scale
        scale.x = glm::max(scale.x, 0.001f);
        scale.y = glm::max(scale.y, 0.001f);
        scale.z = glm::max(scale.z, 0.001f);
        transform->scale = scale;

        ImGui::Unindent();
        ImGui::Spacing();
    }

    ImGui::PopID();
}

void InspectorPanel::drawCameraComponent(Entity entity)
{
    if (!world.hasComponents<CCamera>(entity))
        return;

    auto [e, camera] = world.fetchFrom<CCamera>(entity);

    ImGui::PushID("Camera");
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

    bool open = ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen);

    ImGui::PopStyleVar();

    if (open)
    {
        ImGui::Indent();

        drawFloatControl("FOV", camera->fov, 0.5f, 1.0f, 179.0f);
        drawFloatControl("Near Plane", camera->nearPlane, 0.01f, 0.01f, camera->farPlane);
        drawFloatControl("Far Plane", camera->farPlane, 1.0f, camera->nearPlane, 10000.0f);

        ImGui::Text("Active");
        ImGui::SameLine(100.0f);
        ImGui::Checkbox("##Active", &camera->isActive);

        ImGui::Unindent();
        ImGui::Spacing();
    }

    ImGui::PopID();
}

void InspectorPanel::drawDirectionalLightComponent(Entity entity)
{
    if (!world.hasComponents<CDirectionalLight>(entity))
        return;

    auto [e, light] = world.fetchFrom<CDirectionalLight>(entity);

    ImGui::PushID("DirectionalLight");
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

    bool open = ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen);

    ImGui::PopStyleVar();

    if (open)
    {
        ImGui::Indent();

        drawVec3Control("Direction", light->direction);

        // Normalize direction
        if (glm::length(light->direction) > 0.001f)
            light->direction = glm::normalize(light->direction);

        ImGui::Text("Color");
        ImGui::SameLine(100.0f);
        ImGui::SetNextItemWidth(-1);
        ImGui::ColorEdit3("##Color", glm::value_ptr(light->color));

        drawFloatControl("Ambient", light->ambient, 0.01f);
        drawFloatControl("Intensity", light->intensity, 0.1f);

        ImGui::Unindent();
        ImGui::Spacing();
    }

    ImGui::PopID();
}

void InspectorPanel::drawPointLightComponent(Entity entity)
{
    if (!world.hasComponents<CPointLight>(entity))
        return;

    auto [e, light] = world.fetchFrom<CPointLight>(entity);

    ImGui::PushID("PointLight");
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

    bool open = ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_DefaultOpen);

    ImGui::PopStyleVar();

    if (open)
    {
        ImGui::Indent();

        ImGui::Text("Color");
        ImGui::SameLine(100.0f);
        ImGui::SetNextItemWidth(-1);
        ImGui::ColorEdit3("##Color", glm::value_ptr(light->color));

        drawFloatControl("Intensity", light->intensity, 0.1f);

        ImGui::Unindent();
        ImGui::Spacing();
    }

    ImGui::PopID();
}

const std::vector<std::string> k_materialTypes = {
    "PBR Material",
    "Unlit Material"};

const std::vector<std::string> k_meshTypes = {
    "Cube",
    "Sphere",
    "Plane"};

void InspectorPanel::drawMeshComponent(Entity entity)
{
    if (!world.hasComponents<CMesh>(entity))
        return;

    auto [e, mesh] = world.fetchFrom<CMesh>(entity);
    if (!mesh->meshRef)
    {
        mesh->meshRef = services.get<MeshFactory>()->Cube();
    }
    if (!mesh->material.isValid())
    {
        mesh->material = services.get<MaterialFactory>()->PBRMaterial({});
    }

    ImGui::PushID("MeshRenderer");
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

    bool open = ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen);

    ImGui::PopStyleVar();

    if (open)
    {
        ImGui::Indent();

        ImGui::Text("Mesh");
        ImGui::SameLine(100.0f);
        ImGui::Text("%d", mesh->meshRef);
        ImGui::SameLine();
        if (ImGui::Button("Change Mesh"))
        {
            ImGui::OpenPopup("MeshSelectionPopup");
        }

        if (ImGui::BeginPopup("MeshSelectionPopup"))
        {
            ImGui::SeparatorText("Select Mesh");
            for (const auto &name : k_meshTypes)
            {
                if (ImGui::MenuItem(name.c_str()))
                {
                    services.get<Renderer>()->freeMesh(mesh->meshRef);
                    if (name == "Cube")
                        mesh->meshRef = services.get<MeshFactory>()->Cube();
                    else if (name == "Sphere")
                        mesh->meshRef = services.get<MeshFactory>()->Sphere();
                    else if (name == "Plane")
                        mesh->meshRef = services.get<MeshFactory>()->Plane();
                }
            }
            ImGui::EndPopup();
        }

        // Material properties
        ImGui::SeparatorText("Material properties");
        ImGui::Spacing();

        ImGui::SameLine(100.0f);
        ImGui::SetNextItemWidth(-1);

        drawVec3Control("Base Color", *mesh->material.getVec3("material.baseColor"), 1.0f, 100.0f, 0.0f, 1.0f);
        drawFloatControl("Metallic", *mesh->material.getFloat("material.metallic"), 0.01f, 0.0f, 1.0f);
        drawFloatControl("Roughness", *mesh->material.getFloat("material.roughness"), 0.01f, 0.0f, 1.0f);
        drawFloatControl("AO", *mesh->material.getFloat("material.ao"), 0.01f, 0.0f, 1.0f);
        drawLoadResourcePopup("Base Color Texture",
                              [this, mesh](const std::string &path)
                              {
                                  setMaterialTexture(mesh, "material.baseColorMap", path);
                              });

        bool *useMetallicRoughnessMap = mesh->material.getBool("material.useMetallicRoughnessMap");
        ImGui::Checkbox("Use Metallic-Roughness Texture", useMetallicRoughnessMap);

        if (*useMetallicRoughnessMap)
        {
            drawLoadResourcePopup("Metallic-Roughness Texture",
                                  [this, mesh](const std::string &path)
                                  {
                                      setMaterialTexture(mesh, "material.metallicRoughnessMap", path);
                                  });
        }
        else
        {
            drawLoadResourcePopup("Metallic Texture",
                                  [this, mesh](const std::string &path)
                                  {
                                      setMaterialTexture(mesh, "material.metallicMap", path);
                                  });
            drawLoadResourcePopup("Roughness Texture",
                                  [this, mesh](const std::string &path)
                                  {
                                      setMaterialTexture(mesh, "material.roughnessMap", path);
                                  });
        }
        drawLoadResourcePopup("AO Texture",
                              [this, mesh](const std::string &path)
                              {
                                  setMaterialTexture(mesh, "material.aoMap", path);
                              });

        ImGui::Unindent();
        ImGui::Spacing();
    }

    ImGui::PopID();
}

void InspectorPanel::drawModelComponent(Entity entity)
{
    if (!world.hasComponents<CModel>(entity))
        return;

    auto [e, renderer] = world.fetchFrom<CModel>(entity);

    ImGui::PushID("ModelRenderer");
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

    bool open = ImGui::CollapsingHeader("Model Renderer", ImGuiTreeNodeFlags_DefaultOpen);

    ImGui::PopStyleVar();

    if (open)
    {
        ImGui::Indent();

        ImGui::Text("Model");
        ImGui::SameLine(100.0f);
        ImGui::TextDisabled("(Model data)");

        ImGui::Text("Material Count");
        ImGui::SameLine(100.0f);
        ImGui::Text("%zu", renderer->materialOverrides.size());

        ImGui::Unindent();
        ImGui::Spacing();
    }

    ImGui::PopID();
}

void InspectorPanel::drawEnvironmentComponent(Entity entity)
{
    if (!world.hasComponents<CEnvironment>(entity))
        return;

    auto [e, environment] = world.fetchFrom<CEnvironment>(entity);

    ImGui::PushID("Environment");
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

    bool open = ImGui::CollapsingHeader("Environment", ImGuiTreeNodeFlags_DefaultOpen);

    ImGui::PopStyleVar();

    if (open)
    {
        ImGui::Indent();

        ImGui::Text("Skybox");
        ImGui::SameLine(100.0f);
        ImGui::TextDisabled("(Cubemap data)");

        ImGui::Unindent();
        ImGui::Spacing();
    }

    ImGui::PopID();
}

void InspectorPanel::setMaterialTexture(CMesh *mesh, const std::string &uniformName, const std::string &path)
{
    TextureRef newTexture = services.get<TextureFactory>()->Texture2D(path);
    if (newTexture)
    {
        auto *texUniform = mesh->material.getSampler2D(uniformName);
        if (texUniform)
        {
            services.get<Renderer>()->freeTexture(texUniform->textureRef);
            texUniform->textureRef = newTexture;
        }
        else
        {
            mesh->material.uniforms[uniformName] = Sampler2D{newTexture};
        }
    }
}

void InspectorPanel::drawVec3Control(const char *label, glm::vec3 &values, float resetValue, float columnWidth, float min, float max)
{
    ImGui::PushID(label);

    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text("%s", label);
    ImGui::NextColumn();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));

    float lineHeight = ImGui::GetFontBaked()->Size + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    float widthEach = (ImGui::GetContentRegionAvail().x - (buttonSize.x * 3) - (ImGui::GetStyle().ItemSpacing.x * 5)) / 3.0f;

    // X
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(widthEach);
    ImGui::DragFloat("##X", &values.x, 0.1f);
    ImGui::SameLine();

    // Y
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(widthEach);
    ImGui::DragFloat("##Y", &values.y, 0.1f);
    ImGui::SameLine();

    // Z
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(widthEach);
    ImGui::DragFloat("##Z", &values.z, 0.1f);

    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();

    if (max > min)
    {
        values.x = glm::clamp(values.x, min, max);
        values.y = glm::clamp(values.y, min, max);
        values.z = glm::clamp(values.z, min, max);
    }
}

void InspectorPanel::drawFloatControl(const char *label, float &value, float speed, float min, float max)
{
    ImGui::Text("%s", label);
    ImGui::SameLine(100.0f);
    ImGui::SetNextItemWidth(-1);

    if (max > min)
        ImGui::DragFloat(("##" + std::string(label)).c_str(), &value, speed, min, max);
    else
        ImGui::DragFloat(("##" + std::string(label)).c_str(), &value, speed);
}

void InspectorPanel::drawLoadResourcePopup(const char *label, std::function<void(const std::string &)> onLoad)
{
    if (ImGui::Button(label))
    {
        ImGui::OpenPopup((std::string(label) + "Popup").c_str());
    }

    ImGui::SetNextWindowSize(ImVec2(500, 150), ImGuiCond_Appearing);
    if (ImGui::BeginPopupModal((std::string(label) + "Popup").c_str(), nullptr, ImGuiWindowFlags_NoResize))
    {
        ImGui::Spacing();
        ImGui::SeparatorText("Load Resource");
        ImGui::Spacing();

        ImGui::Text("Resource path:");
        ImGui::Spacing();

        ImGui::SetNextItemWidth(-1);
        if (ImGui::InputText("##ResourcePath", m_pathBuffer, sizeof(m_pathBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            onLoad(std::string(m_pathBuffer));
            ImGui::CloseCurrentPopup();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        float buttonWidth = 100.0f;
        float spacing = ImGui::GetStyle().ItemSpacing.x;
        float totalWidth = (buttonWidth * 2) + spacing;
        float indent = (ImGui::GetContentRegionAvail().x - totalWidth) * 0.5f;

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + indent);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.5f, 0.15f, 1.0f));
        if (ImGui::Button("Load", ImVec2(buttonWidth, 0)))
        {
            Log::Print("Loading resource: " + std::string(m_pathBuffer), LogLevel::Info);
            onLoad(std::string(m_pathBuffer));
            ImGui::CloseCurrentPopup();
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(buttonWidth, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}