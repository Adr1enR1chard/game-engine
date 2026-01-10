#include <engine/Core.hpp>

class MovingPointLightSystem : public System
{
  public:
    void update(World& world, double deltaTime) override
    {
        for (const Entity& entity : world.getEntitiesWithComponents<CTransform, CPointLight>()) {
            auto&     transform = world.getComponent<CTransform>(entity);
            glm::vec3 pos       = transform.position;
            pos.x               = 5.0f * std::sin(static_cast<float>(timeAccumulator));
            pos.z               = 5.0f * std::cos(static_cast<float>(timeAccumulator));
            transform.position  = pos;
        }
        timeAccumulator += static_cast<float>(deltaTime);
    }

  private:
    float timeAccumulator = 0.0f;
};

class PrintFPSSystem : public System
{
  public:
    void update(World& /*world*/, double /*deltaTime*/) override
    {
        using clock = std::chrono::steady_clock;

        static auto lastReport = clock::now();
        static int  frames     = 0;

        frames++;

        auto                          now     = clock::now();
        std::chrono::duration<double> elapsed = now - lastReport;

        if (elapsed.count() >= 1.0) {
            double fps = frames / elapsed.count();
            std::cout << "FPS: " << fps << std::endl;

            frames     = 0;
            lastReport = now;
        }
    }
};

class StartupSystem : public System
{
  public:
    void start(World& world) override
    {
        world.Serv<Window>().setClearColor(glm::vec3(0.1f, 0.1f, 0.1f));
        world.createEntity(CCamera{}, CTransform{.position = glm::vec3(0.0f, 0.0f, 3.0f)});
        world.createEntity(CDirectionalLight{.direction = glm::vec3(-0.2f, -1.0f, -0.3f),
                                             .color     = glm::vec3(1.0f, 1.0f, 1.0f),
                                             .ambient   = 0.2f,
                                             .intensity = 0.5f});

        // TODO: Fix Mesh rendering. Actually only models are rendered properly.
        world.createEntity(CPointLight{.color = glm::vec3(1.0f, 0.5f, 0.5f), .intensity = 5.0f, .radius = 1.0f},
                           CTransform{.position = glm::vec3(-2.0f, 2.0f, -2.0f), .scale = glm::vec3(0.2f)},
                           CMeshRenderer{
                               .mesh     = Mesh::Cube(),
                               .material = Material::Default(),
                           });

        world.createEntity(CTransform{.position = glm::vec3(0.0f, 0, -6.0f), .scale = glm::vec3(0.01f)},
                           CMeshRenderer{
                               .material = Material::Default()->setTexture(
                                   "albedoMap", Texture("assets/textures/backpack/baseColor.jpeg")),
                               .model = Model("assets/models/backpack.gltf"),
                           });
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
    engine.systems().registerSystem<PrintFPSSystem>();
    engine.systems().registerSystem<StartupSystem>(SystemPhase::Start);

    engine.run();
    return 0;
}