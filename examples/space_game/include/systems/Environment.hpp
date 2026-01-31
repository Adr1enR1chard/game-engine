#pragma once

#include <DefaultBundle.hpp>

class Environment : public System
{
public:
    void start() override
    {
        ShaderRef skyboxShader = services().get<ShaderFactory>()->CustomShader("SpaceSkyboxShader", "assets/shaders/space_skybox.vert", "assets/shaders/space_skybox.frag",
                                                                               {
                                                                                   {"uSunDirection", glm::vec3(-1.0f, 0.0f, 0.0f)},
                                                                                   {"uSunColor", glm::vec3(1.0f, 1.0f, 1.0f)},
                                                                                   {"uSunSize", 0.1f},
                                                                                   {"uPlanetSize", 0.5f},
                                                                               },
                                                                               {
                                                                                   .cullFaceEnabled = false,
                                                                                   .depthTestEnabled = false,
                                                                                   .depthWriteEnabled = false,
                                                                               });

        if (skyboxShader == 0)
            Log::Print("Failed to create space skybox shader", LogLevel::Error);

        m_environmentEntity = world().createEntity(CEnvironment{
            .skyboxMaterial = services().get<MaterialFactory>()->CustomMaterial(skyboxShader)});

        world().createEntity(CDirectionalLight{
            .direction = glm::vec3(1.0f, 0.0f, 0.0f),
            .color = glm::vec3(1.0f, 1.0f, 1.0f),
            .ambient = 1.0f,
            .intensity = 100.0f,
        });
    }

private:
    Entity m_environmentEntity;
};