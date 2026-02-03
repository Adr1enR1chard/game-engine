#include <DefaultBundle.hpp>
#include <systems/CameraControlSystem.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

using namespace engine;
using namespace default_bundle;

class MySystem : public System
{
public:
    void start() override
    {
        world().createEntity(CCamera{}, CTransform{glm::vec3(0.0f, 0.0f, 3.0f)});
        m_dirLight = world().createEntity(CDirectionalLight{glm::vec3(-3.0f, -1.0f, -1.0f), glm::vec3(1.0f), 5.0f, 10.0f});
        world().createEntity(CEnvironment{.skyboxMaterial = services().get<MaterialFactory>()->SkyboxMaterial({})});

        m_metalSphere = world().createEntity(
            CMeshRenderer{
                .meshRef = services().get<MeshFactory>()->Sphere(),
                .material = services().get<MaterialFactory>()->PBRMaterial({
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
                .material = services().get<MaterialFactory>()->PBRMaterial({
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
                .model = services().get<ModelFactory>()->LoadModel("assets/models/sword/scene.gltf"),
            },
            CTransform{glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, glm::radians(40.0f), 0.0f)), glm::vec3(1.0f)});

        world().createEntity(
            CMeshRenderer({
                .meshRef = services().get<MeshFactory>()->Plane(),
                .material = services().get<MaterialFactory>()->PBRMaterial({.metallic = 0.0f,
                                                                            .roughness = 1.0f,
                                                                            .ao = 0.0f}),
            }),
            CTransform{glm::vec3(0.0f, -1.0f, 0.0f), glm::quat(glm::vec3(glm::radians(180.0f), 0.0f, 0.0f)), glm::vec3(10.0f)});
    }

    void update(float deltaTime) override
    {
        float angle = static_cast<float>(deltaTime) * 100.0f;
        if (auto [_, transform] = world().fetchFrom<CTransform>(m_metalSphere); transform)
        {
            transform->rotate(glm::radians(angle), glm::vec3(0, 1, 0));
        }
        if (auto [_, transform] = world().fetchFrom<CTransform>(m_groundSphere); transform)
        {
            transform->rotate(glm::radians(angle), glm::vec3(0, 1, 0));
        }
        if (auto [_, transform] = world().fetchFrom<CTransform>(m_sword); transform)
        {
            transform->rotate(glm::radians(angle), glm::vec3(1, 0, 0));
        }
        if (auto [_, dirLight] = world().fetchFrom<CDirectionalLight>(m_dirLight); dirLight)
        {
            dirLight->direction = glm::rotateY(dirLight->direction, glm::radians(angle / 3.0f));
        }
    }

private:
    Entity m_dirLight = 0;
    Entity m_metalSphere = 0;
    Entity m_groundSphere = 0;
    Entity m_sword = 0;
};

int main()
{
    Engine::Init().addBundle<DefaultBundle>().addSystems<CameraControlSystem, MySystem>().run(1280, 720, "Basic Scene", false);

    return 0;
}