#include <DefaultBundle.hpp>
#include <systems/CameraControlSystem.hpp>

class MySystem : public System
{
  public:
    void start() override
    {
        world().create(CCamera{}, CTransform{glm::vec3(0.0f, 0.0f, 3.0f)});
        world().create(CDirectionalLight{glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f), 2.0f, 10.0f});
        world().create(CEnvironment{.skyboxMaterial = services().get<MaterialFactory>()->Skybox({})});

        m_metalSphere = world().create(
            CMeshRenderer{
                .meshRef     = services().get<MeshFactory>()->Sphere(),
                .materialRef = services().get<MaterialFactory>()->PBR({
                    .ao = 0.2f,
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
        m_groundSphere = world().create(
            CMeshRenderer{
                .meshRef     = services().get<MeshFactory>()->Sphere(),
                .materialRef = services().get<MaterialFactory>()->PBR({
                    .metallic     = 0.0f,
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
    }

    void update(double deltaTime) override
    {
        float angle = static_cast<float>(deltaTime) * 100.0f;
        if (auto [_, transform] = world().getFrom<CTransform>(m_metalSphere); transform) {
            transform->rotation.y += angle;
        }
        if (auto [_, transform] = world().getFrom<CTransform>(m_groundSphere); transform) {
            transform->rotation.y += angle;
        }
    }

  private:
    Entity m_metalSphere  = 0;
    Entity m_groundSphere = 0;
};

int main()
{
    Engine::Init().addBundle<DefaultBundle>().addSystems<CameraControlSystem, MySystem>().run();

    return 0;
}