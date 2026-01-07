#include <engine/Engine.hpp>
#include <engine/SceneManager.hpp>
#include <engine/Time.hpp>
#include <engine/component/CTransform.hpp>
#include <engine/component/CMeshRenderer.hpp>
#include <engine/component/CCamera.hpp>
#include <engine/system/TransformSystem.hpp>
#include <engine/system/RenderSystem.hpp>
#include <engine/system/CameraSystem.hpp>

class RotatingCubeSystem : public System
{
public:
    void update(EngineContext &engineContext) override
    {
        Scene &scene = engineContext.getService<SceneManager>().currentScene();
        float deltaTime = engineContext.getService<Time>().deltaTime();
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
    void update(EngineContext &engineContext) override
    {
        static float elapsedTime = 0.0f;
        static int frameCount = 0;

        float deltaTime = engineContext.getService<Time>().deltaTime();
        elapsedTime += deltaTime;
        frameCount++;

        if (elapsedTime >= 1.0f)
        {
            std::cout << "FPS: " << frameCount << std::endl;
            elapsedTime = 0.0f;
            frameCount = 0;
        }
    }
};

int main()
{
    Engine engine(800, 600, "Game Engine");

    Scene &scene = engine.context().getService<SceneManager>().currentScene();
    scene.systems().registerSystem<TransformSystem>();
    scene.systems().registerSystem<CameraSystem>();
    scene.systems().registerSystem<RenderSystem>();
    scene.systems().registerSystem<RotatingCubeSystem>();
    scene.systems().registerSystem<PrintFPSSystem>();

    // cube data
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

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

    std::shared_ptr<MeshHandle> mesh = std::make_shared<MeshHandle>(vertices, sizeof(vertices) / sizeof(float));
    std::shared_ptr<MaterialHandle> material = std::make_shared<MaterialHandle>(
        "assets/shaders/basic.vs",
        "assets/shaders/basic.fs");
    material->addTexture(TextureHandle("assets/textures/container.jpg"));
    material->addTexture(TextureHandle("assets/textures/pixel_logo.png"), "faceTexture");

    for (const glm::vec3 &pos : cubePositions)
    {
        Entity cube = scene.registry().createEntity();
        scene.registry().createComponent<CTransform>(cube).setPosition(pos);
        scene.registry().createComponent<CMeshRenderer>(cube).setMesh(mesh).setMaterial(material);
    }

    Entity camera = scene.registry().createEntity();
    scene.registry().createComponent<CCamera>(camera);
    scene.registry().createComponent<CTransform>(camera).setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    engine.run();
    return 0;
}