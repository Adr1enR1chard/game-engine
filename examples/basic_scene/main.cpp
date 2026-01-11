#include <engine/Core.hpp>

struct CBackpack {
};

class MovingPointLightSystem : public System
{
  public:
    void update(World& world, double deltaTime) override
    {
        for (const auto& [entity, transform, pointLight] : world.get<CTransform, CPointLight>()) {
            glm::vec3 pos       = transform->position;
            pos.x               = 5.0f * std::sin(static_cast<float>(timeAccumulator));
            pos.z               = 5.0f * std::cos(static_cast<float>(timeAccumulator));
            transform->position = pos;
        }
        timeAccumulator += static_cast<float>(deltaTime);

        // const auto& [entity, backpack, transform] = world.getAt<CBackpack, CTransform>(0);
        // if (entity) {
        //     transform->rotation.y += 50.0f * static_cast<float>(deltaTime);
        // }
    }

  private:
    float timeAccumulator = 0.0f;
};

class StartupSystem : public System
{
  public:
    void start(World& world) override
    {
        world.Serv<Window>().setClearColor(glm::vec3(0.1f, 0.1f, 0.1f));
        world.create(CCamera{}, CTransform{.position = glm::vec3(0.0f, 0.0f, 3.0f)});
        world.create(CDirectionalLight{.direction = glm::vec3(-0.2f, -1.0f, -0.3f),
                                       .color     = glm::vec3(1.0f, 1.0f, 1.0f),
                                       .ambient   = 0.2f,
                                       .intensity = 0.0f});

        world.create(CPointLight{.color = glm::vec3(1.0f, 0.2f, 0.2f), .intensity = 100.0f},
                     CTransform{.position = glm::vec3(-2.0f, 2.0f, -2.0f), .scale = glm::vec3(0.2f)},
                     CMeshRenderer{
                         .mesh     = Mesh::Cube(),
                         .material = MaterialInstance::Default(),
                     });

        world.create(CTransform{.position = glm::vec3(0.0f, 0, -6.0f), .scale = glm::vec3(0.01f)},
                     CModelRenderer{
                         .model             = Model("assets/models/backpack.gltf"),
                         .materialOverrides = std::vector<MaterialInstance>{MaterialInstance::Default({
                             .ao           = 0.0f,
                             .albedoMap    = Texture("assets/textures/backpack/albedo.jpeg"),
                             .metallicMap  = Texture("assets/textures/backpack/metallic.png"),
                             .roughnessMap = Texture("assets/textures/backpack/roughness.png"),
                         })},
                     },
                     CBackpack{});
    }
};

int main()
{
    Engine engine(800, 600, "Game Engine");

    engine.systems().registerSystem<TransformSystem>();
    engine.systems().registerSystem<CameraSystem>();
    engine.systems().registerSystem<RenderSystem>();
    engine.systems().registerSystem<LightSystem>();
    engine.systems().registerSystem<MovingPointLightSystem>();
    engine.systems().registerSystem<StartupSystem>(SystemPhase::Start);

    engine.run();
    return 0;
}