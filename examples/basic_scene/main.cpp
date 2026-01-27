#include <DefaultBundle.hpp>
#include <systems/CameraControlSystem.hpp>

class MySystem : public System
{
public:
    void start() override
    {
        services().get<Window>()->setSize(1280, 720);
        world().createEntity(CCamera{}, CTransform{glm::vec3(0.0f, 0.0f, 3.0f)});
        world().createEntity(CDirectionalLight{glm::vec3(-3.0f, -1.0f, -1.0f), glm::vec3(1.0f), 10.0f, 10.0f});
        world().createEntity(CEnvironment{.skyboxMaterial = services().get<MaterialFactory>()->Skybox({})});

        m_metalSphere = world().createEntity(
            CMeshRenderer{
                .meshRef = services().get<MeshFactory>()->Sphere(),
                .materialRef = services().get<MaterialFactory>()->PBR({
                    .ao = 0.1f,
                    .baseColorMap =
                        services().get<TextureFactory>()->Texture2D("assets/textures/metal/Metal055A_1K-JPG_Color.jpg"),
                    .normalMap = services().get<TextureFactory>()->Texture2D(
                        "assets/textures/metal/Metal055A_1K-JPG_NormalGL.jpg"),
                    .metallicMap = services().get<TextureFactory>()->Texture2D(
                        "assets/textures/metal/Metal055A_1K-JPG_Metalness.jpg"),
                    .roughnessMap = services().get<TextureFactory>()->Texture2D(
                        "assets/textures/metal/Metal055A_1K-JPG_Roughness.jpg"),
                }),
            },
            CTransform{glm::vec3(-1.0f, 0.0f, 0.0f)});

        m_groundSphere = world().createEntity(
            CMeshRenderer{
                .meshRef = services().get<MeshFactory>()->Sphere(),
                .materialRef = services().get<MaterialFactory>()->PBR({
                    .metallic = 0.0f,
                    .baseColorMap = services().get<TextureFactory>()->Texture2D(
                        "assets/textures/ground/Ground104_1K-JPG_Color.jpg"),
                    .normalMap = services().get<TextureFactory>()->Texture2D(
                        "assets/textures/ground/Ground104_1K-JPG_NormalGL.jpg"),
                    .roughnessMap = services().get<TextureFactory>()->Texture2D(
                        "assets/textures/ground/Ground104_1K-JPG_Roughness.jpg"),
                    .aoMap = services().get<TextureFactory>()->Texture2D(
                        "assets/textures/ground/Ground104_1K-JPG_AmbientOcclusion.jpg"),
                }),
            },
            CTransform{glm::vec3(1.0f, 0.0f, 0.0f)});

        m_sword = world().createEntity(
            CModelRenderer{
                .modelRef = services().get<ModelFactory>()->LoadModel("assets/models/sword/scene.gltf"),
            },
            CTransform{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 45.0f, 0.0f), glm::vec3(1.0f)});
    }

    void update(double deltaTime) override
    {
        float angle = static_cast<float>(deltaTime) * 100.0f;
        if (auto [_, transform] = world().fetchFrom<CTransform>(m_metalSphere); transform)
        {
            transform->rotation.y += angle;
        }
        if (auto [_, transform] = world().fetchFrom<CTransform>(m_groundSphere); transform)
        {
            transform->rotation.y += angle;
        }
        if (auto [_, transform] = world().fetchFrom<CTransform>(m_sword); transform)
        {
            transform->rotation.x += angle;
        }
    }

private:
    Entity m_metalSphere = 0;
    Entity m_groundSphere = 0;
    Entity m_sword = 0;
};

int main()
{
    Engine::Init().addBundle<DefaultBundle>().addSystems<CameraControlSystem, MySystem>().run();

    return 0;
}