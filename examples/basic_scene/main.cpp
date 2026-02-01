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
        m_dirLight = world().createEntity(CDirectionalLight{glm::vec3(-3.0f, -1.0f, -1.0f), glm::vec3(1.0f), 10.0f, 100.0f});
        world().createEntity(CEnvironment{.skyboxMaterial = services().get<MaterialFactory>()->SkyboxMaterial({})});

        m_metalSphere = world().createEntity(
            CMeshRenderer{
                .meshRef = services().get<MeshFactory>()->Sphere(),
                .materialRef = services().get<MaterialFactory>()->PBRMaterial({
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
                .materialRef = services().get<MaterialFactory>()->PBRMaterial({
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

        world().createEntity(
            CMeshRenderer({
                .meshRef = services().get<MeshFactory>()->Plane(),
                .materialRef = services().get<MaterialFactory>()->PBRMaterial({.metallic = 0.0f,
                                                                               .roughness = 1.0f,
                                                                               .ao = 0.0f}),
            }),
            CTransform{glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(10.0f)});
    }

    void update(float deltaTime) override
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
        if (auto [_, dirLight] = world().fetchFrom<CDirectionalLight>(m_dirLight); dirLight)
        {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle / 2), glm::vec3(0.0f, 1.0f, 0.0f));
            dirLight->direction = glm::vec3(rotation * glm::vec4(dirLight->direction, 0.0f));
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