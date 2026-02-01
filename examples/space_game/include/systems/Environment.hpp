#pragma once

#include <DefaultBundle.hpp>
#include <random>
#include <glm/gtc/random.hpp>

using namespace engine;
using namespace default_bundle;

class SpaceEnvironmentSystem : public System
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
            .intensity = 10.0f,
        });

        ModelRef planetModel = services().get<ModelFactory>()->LoadModel("assets/models/planet/planet.gltf");

        // Spawn multiple random planets
        for (int i = 0; i < 50; ++i)
        {
            glm::vec3 position = glm::sphericalRand(500.0f);
            float scaleValue = static_cast<float>(rand() % 50 + 5);
            glm::vec3 scale = glm::vec3(scaleValue);

            world().createEntity(
                CModelRenderer{
                    .modelRef = planetModel,
                },
                CTransform{
                    .position = position,
                    .scale = scale,
                });
        }
    }

private:
    Entity m_environmentEntity;
};