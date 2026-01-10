#include <engine/Core.hpp>

class MovingPointLightSystem : public System
{
  public:
    void update(World& world, double deltaTime) override
    {
        for (const Entity& entity : world.getEntitiesWithComponents<CTransform, CPointLight>()) {
            auto&     transform = world.getComponent<CTransform>(entity);
            glm::vec3 pos       = transform.getPosition();
            pos.x               = 5.0f * std::sin(static_cast<float>(timeAccumulator));
            pos.z               = 5.0f * std::cos(static_cast<float>(timeAccumulator));
            transform.setPosition(pos);
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

int main()
{
    Engine engine(800, 600, "Game Engine");
    engine.world().Serv<Window>().setClearColor(glm::vec3(0.1f, 0.1f, 0.1f));

    engine.systems().registerSystem<TransformSystem>();
    engine.systems().registerSystem<CameraSystem>();
    engine.systems().registerSystem<RenderSystem>();
    engine.systems().registerSystem<LightSystem>();
    engine.systems().registerSystem<MovingPointLightSystem>();
    engine.systems().registerSystem<PrintFPSSystem>();

    Entity camera = engine.world().createEntity();
    engine.world().createComponent<CCamera>(camera);
    engine.world().createComponent<CTransform>(camera).setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    Entity             dirLight         = engine.world().createEntity();
    CDirectionalLight& directionalLight = engine.world().createComponent<CDirectionalLight>(dirLight);

    directionalLight.direction = glm::vec3(0.0f, 0.0f, -1.0f);
    directionalLight.color     = glm::vec3(1.0f, 1.0f, 1.0f);
    directionalLight.ambient   = 0.2f;
    directionalLight.intensity = 0.2f;

    Entity                    pointLight          = engine.world().createEntity();
    CPointLight&              pointLightComp      = engine.world().createComponent<CPointLight>(pointLight);
    std::shared_ptr<Material> lightMaterial       = Material::Default();
    auto&                     pointLightTransform = engine.world().createComponent<CTransform>(pointLight);
    engine.world().createComponent<CMeshRenderer>(pointLight).setMesh(Mesh::Cube()).setMaterial(lightMaterial);
    pointLightTransform.setPosition(glm::vec3(2.0f, 2.0f, 2.0f));
    pointLightTransform.setScale(glm::vec3(0.2f));

    pointLightComp.color     = glm::vec3(0.2f, 1.0f, 0.2f);
    pointLightComp.intensity = 10.0f;
    pointLightComp.radius    = 1.0f;

    lightMaterial->setUniform("albedo", glm::vec3(1.0f, 1.0f, 0.0f));

    Model backpackModel("assets/models/backpack.gltf");

    Entity backpack          = engine.world().createEntity();
    auto&  backpackTransform = engine.world().createComponent<CTransform>(backpack);
    backpackTransform.setPosition(glm::vec3(0.0f, 0, -6.0f));
    backpackTransform.setScale(glm::vec3(0.01f));
    // backpackTransform.setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
    auto& backpackMeshRenderer = engine.world().createComponent<CMeshRenderer>(backpack);
    backpackMeshRenderer.setModel(backpackModel);
    backpackMeshRenderer.setMaterial(Material::Default());
    backpackMeshRenderer.getMaterial().setTexture("albedoMap", Texture("assets/textures/backpack/baseColor.jpeg"));
    engine.run();
    return 0;
}