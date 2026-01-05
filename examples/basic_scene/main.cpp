#include <engine/Engine.hpp>
#include "ecs/component/CTransform.hpp"
#include "ecs/component/CTransformCache.hpp"
#include "ecs/component/CMeshRenderer.hpp"
#include <ecs/system/TransformSystem.hpp>
#include <ecs/system/RenderSystem.hpp>
#include <ecs/component/CCamera.hpp>
#include <ecs/component/CCameraTransform.hpp>

class RotatingCubeSystem : public System
{
public:
    void update(EngineContext &engineContext) override
    {
        Scene &scene = engineContext.getService<SceneManager>().currentScene();
        float deltaTime = engineContext.getService<Time>().deltaTime();
        for (const Entity &entity : scene.registry().getEntitiesWithComponent<CTransform>())
        {
            auto &transform = scene.registry().getComponent<CTransform>(entity);
            transform.rotation.y += 50.0f * deltaTime;
            transform.dirty = true;
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

    std::shared_ptr<MeshHandle> rendererMesh = std::make_shared<MeshHandle>(vertices, sizeof(vertices) / sizeof(float));
    std::shared_ptr<MaterialHandle> ourMaterial = std::make_shared<MaterialHandle>(
        "assets/shaders/basic.vs",
        "assets/shaders/basic.fs");
    ourMaterial->addTexture(TextureHandle("assets/textures/container.jpg"))
        .filteringParameters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    ourMaterial->addTexture(TextureHandle("assets/textures/pixel_logo.png"), "faceTexture")
        .filteringParameters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    for (const glm::vec3 &pos : cubePositions)
    {
        Entity cube = scene.registry().createEntity();
        auto &transform = scene.registry().createComponent<CTransform>(cube);
        transform.position = pos;
        transform.dirty = true;
        CMeshRenderer &meshRenderer = scene.registry().createComponent<CMeshRenderer>(cube);
        meshRenderer.mesh = rendererMesh;
        meshRenderer.material = ourMaterial;
    }

    Entity camera = scene.registry().createEntity();
    scene.registry().createComponent<CCamera>(camera);
    auto &cameraTransform = scene.registry().createComponent<CCameraTransform>(camera);
    cameraTransform.position = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraTransform.isDirty = true;

    engine.run();
    return 0;
}