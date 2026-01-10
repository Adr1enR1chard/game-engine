#include <engine/Core.hpp>

class MovingPointLightSystem : public System
{
  public:
    void update(EngineContext& engineContext, double deltaTime) override
    {
        Scene& scene = engineContext.getService<SceneManager>().currentScene();
        for (const Entity& entity : scene.registry().getEntitiesWithComponents<CTransform, CPointLight>()) {
            auto&     transform = scene.registry().getComponent<CTransform>(entity);
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
    void update(EngineContext& /*engineContext*/, double /*deltaTime*/) override
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
    engine.context().window().setClearColor(glm::vec3(0.1f, 0.1f, 0.1f));

    Scene& scene = engine.context().getService<SceneManager>().currentScene();
    scene.systems().registerSystem<TransformSystem>();
    scene.systems().registerSystem<CameraSystem>();
    scene.systems().registerSystem<RenderSystem>();
    scene.systems().registerSystem<LightSystem>();
    scene.systems().registerSystem<MovingPointLightSystem>();
    scene.systems().registerSystem<PrintFPSSystem>();

    Entity camera = scene.registry().createEntity();
    scene.registry().createComponent<CCamera>(camera);
    scene.registry().createComponent<CTransform>(camera).setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    Entity             dirLight         = scene.registry().createEntity();
    CDirectionalLight& directionalLight = scene.registry().createComponent<CDirectionalLight>(dirLight);

    directionalLight.direction = glm::vec3(0.0f, 0.0f, -1.0f);
    directionalLight.color     = glm::vec3(1.0f, 1.0f, 1.0f);
    directionalLight.ambient   = 0.2f;
    directionalLight.intensity = 0.2f;

    Entity                    pointLight          = scene.registry().createEntity();
    CPointLight&              pointLightComp      = scene.registry().createComponent<CPointLight>(pointLight);
    std::shared_ptr<Material> lightMaterial       = Material::Default();
    auto&                     pointLightTransform = scene.registry().createComponent<CTransform>(pointLight);
    scene.registry().createComponent<CMeshRenderer>(pointLight).setMesh(Mesh::Cube()).setMaterial(lightMaterial);

    pointLightTransform.setPosition(glm::vec3(2.0f, 2.0f, 2.0f));
    pointLightTransform.setScale(glm::vec3(0.2f));

    pointLightComp.color     = glm::vec3(0.2f, 1.0f, 0.2f);
    pointLightComp.intensity = 10.0f;
    pointLightComp.radius    = 1.0f;

    lightMaterial->setUniform("albedo", glm::vec3(1.0f, 1.0f, 0.0f));

    Model backpackModel("assets/models/backpack.gltf");

    Entity backpack          = scene.registry().createEntity();
    auto&  backpackTransform = scene.registry().createComponent<CTransform>(backpack);
    backpackTransform.setPosition(glm::vec3(0.0f, 0, -6.0f));
    backpackTransform.setScale(glm::vec3(0.01f));
    // backpackTransform.setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
    auto& backpackMeshRenderer = scene.registry().createComponent<CMeshRenderer>(backpack);
    backpackMeshRenderer.setModel(backpackModel);
    backpackMeshRenderer.setMaterial(Material::Default());
    backpackMeshRenderer.getMaterial().setTexture("albedoMap", Texture("assets/textures/backpack/baseColor.jpeg"));
    engine.run();
    return 0;
}