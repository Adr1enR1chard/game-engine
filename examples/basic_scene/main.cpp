#include <engine/Core.hpp>

class RotatingCubeSystem : public System
{
public:
    void update(EngineContext &engineContext, double deltaTime) override
    {
        Scene &scene = engineContext.getService<SceneManager>().currentScene();
        for (const Entity &entity : scene.registry().getEntitiesWithComponents<CTransform, CMeshRenderer>())
        {
            auto &transform = scene.registry().getComponent<CTransform>(entity);
            transform.setRotation(glm::vec3(transform.getRotation().x, transform.getRotation().y + 50.0f * deltaTime, transform.getRotation().z));
        }
    }
};

class PrintFPSSystem : public System
{
public:
    void update(EngineContext & /*engineContext*/, double /*deltaTime*/) override
    {
        using clock = std::chrono::steady_clock;

        static auto lastReport = clock::now();
        static int frames = 0;

        frames++;

        auto now = clock::now();
        std::chrono::duration<double> elapsed = now - lastReport;

        if (elapsed.count() >= 1.0)
        {
            double fps = frames / elapsed.count();
            std::cout << "FPS: " << fps << std::endl;

            frames = 0;
            lastReport = now;
        }
    }
};

int main()
{
    Engine engine(800, 600, "Game Engine");
    engine.context().window().setClearColor(glm::vec3(0.1f, 0.1f, 0.1f));

    Scene &scene = engine.context().getService<SceneManager>().currentScene();
    scene.systems().registerSystem<TransformSystem>();
    scene.systems().registerSystem<CameraSystem>();
    scene.systems().registerSystem<RenderSystem>();
    scene.systems().registerSystem<LightSystem>();
    scene.systems().registerSystem<RotatingCubeSystem>();
    scene.systems().registerSystem<PrintFPSSystem>();

    std::shared_ptr<Mesh> mesh = Mesh::Cube();
    std::shared_ptr<Material> material = Material::Default();
    material->addTexture(Texture("assets/textures/tiles-albedo.jpg"), "albedo");

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    for (const glm::vec3 &pos : cubePositions)
    {
        Entity cube = scene.registry().createEntity();
        scene.registry().createComponent<CTransform>(cube).setPosition(pos);
        scene.registry().createComponent<CMeshRenderer>(cube).setMesh(mesh).setMaterial(material);
    }

    Entity camera = scene.registry().createEntity();
    scene.registry().createComponent<CCamera>(camera);
    scene.registry().createComponent<CTransform>(camera).setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    Entity dirLight = scene.registry().createEntity();
    CDirectionalLight &directionalLight = scene.registry().createComponent<CDirectionalLight>(dirLight);
    directionalLight.direction = glm::vec3(0.0f, 0.0f, -1.0f);
    directionalLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    directionalLight.ambient = 0.2f;
    directionalLight.intensity = 0.8f;

    engine.run();
    return 0;
}