#include <engine/Core.hpp>

class MovingPointLightSystem : public System
{
  public:
    void update(World& world, ServiceRegistry& /*services*/, double deltaTime) override
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
    void start(World& world, ServiceRegistry& /*services*/) override
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
    void update(World& world, ServiceRegistry& /*services*/, double deltaTime) override
    {
        float dt = static_cast<float>(deltaTime);
        if (auto [entity, transform] = world.getFrom<CTransform>(backpackEntity); entity != 0) {
            transform->rotation.y += 100.0f * dt;
        }
    }

  private:
    Entity backpackEntity;
};

class CameraControlSystem : public System
{
  public:
    void update(World& world, ServiceRegistry& services, double deltaTime) override
    {
        float dt = static_cast<float>(deltaTime);
        if (auto [entity, cCam, transform] = world.getAt<CCamera, CTransform>(0); entity != 0) {
            Input* input = services.get<Input>();
            transform->rotation.y += -input->getMouseDelta().x * 0.1f;
            transform->rotation.x += -input->getMouseDelta().y * 0.1f;

            transform->rotation.x = glm::clamp(transform->rotation.x, -89.0f, 89.0f);

            float yaw   = glm::radians(transform->rotation.y);
            float pitch = glm::radians(transform->rotation.x);

            glm::vec3 forward;
            forward.x = -cos(pitch) * sin(yaw);
            forward.y = sin(pitch);
            forward.z = -cos(pitch) * cos(yaw);

            forward = glm::normalize(forward);

            glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

            glm::vec2 direction(0.0f);

            if (input->isKeyDown(Key::A))
                direction.x = -1;
            if (input->isKeyDown(Key::D))
                direction.x = 1;
            if (input->isKeyDown(Key::W))
                direction.y = 1;
            if (input->isKeyDown(Key::S))
                direction.y = -1;

            float speed = 5.0f * dt;

            transform->position += forward * direction.y * speed;
            transform->position += right * direction.x * speed;
        }
    }
};

class StartupSystem : public System
{
  public:
    void start(World& world, ServiceRegistry& services) override
    {
        services.get<Window>()->setClearColor(glm::vec3(0.1f, 0.1f, 0.1f));
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
    Engine engine = Engine();
    engine.addBundle<DefaultBundle>();
    engine.addSystems<MovingPointLightSystem, StartupSystem, BackPackSystem, CameraControlSystem>();
    engine.run<Window>(1280, 720, "Basic Scene Example", false);

    return 0;
}