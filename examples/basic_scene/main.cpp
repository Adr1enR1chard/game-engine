#include <engine/Core.hpp>

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
    }

  private:
    float timeAccumulator = 0.0f;
};

class BackPackSystem : public System
{
  public:
    void start(World& world) override
    {
        backpackEntity = world.create(CTransform{.position = glm::vec3(0.0f, 0, -6.0f), .scale = glm::vec3(0.01f)},
                                      CModelRenderer{
                                          .model             = Model("assets/models/backpack.gltf"),
                                          .materialOverrides = std::vector<MaterialInstance>{MaterialInstance::Default({
                                              .ao           = 0.0f,
                                              .albedoMap    = Texture("assets/textures/backpack/albedo.jpeg"),
                                              .metallicMap  = Texture("assets/textures/backpack/metallic.png"),
                                              .roughnessMap = Texture("assets/textures/backpack/roughness.png"),
                                              .normalMap    = Texture("assets/textures/backpack/normal.png"),
                                          })},
                                      });
    }

  public:
    void update(World& world, double /*deltaTime*/) override
    {
        if (auto [entity, transform] = world.getFrom<CTransform>(backpackEntity); entity != 0) {
            transform->rotation.y += 0.5f;
        }
    }

  private:
    Entity backpackEntity;
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

        world.create(CPointLight{.color = glm::vec3(1.0f, 1.0f, 1.0f), .intensity = 500.0f},
                     CTransform{.position = glm::vec3(-2.0f, 2.0f, -2.0f), .scale = glm::vec3(0.2f)},
                     CMeshRenderer{
                         .mesh     = Mesh::Cube(),
                         .material = MaterialInstance::Default({
                             .albedo    = glm::vec3(1.0f, 1.0f, 1.0f),
                             .metallic  = 0.0f,
                             .roughness = 1.0f,
                         }),
                     });

        world.create(
            CMeshRenderer{
                .mesh     = Mesh::Cube(),
                .material = MaterialInstance::Default({.metallic     = 0.0f,
                                                       .ao           = 0.1f,
                                                       .albedoMap    = Texture("assets/textures/wall/diffuse.png"),
                                                       .roughnessMap = Texture("assets/textures/wall/roughness.png"),
                                                       .aoMap        = Texture("assets/textures/wall/ao.png"),
                                                       .normalMap    = Texture("assets/textures/wall/normal.png")}),
            },
            CTransform{.position = glm::vec3(0.0f, 0.0f, -10.0f), .scale = glm::vec3(20.0f, 20.0f, 1.0f)});
    }
};

int main()
{
    Engine engine(1920, 1080, "Game Engine");

    engine.use<DefaultBundle>().add<MovingPointLightSystem, StartupSystem, BackPackSystem>().run();

    return 0;
}