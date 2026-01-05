#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "handle/ShaderHandle.hpp"
#include "handle/TextureHandle.hpp"
#include "ecs/component/CMeshRenderer.hpp"
#include "ecs/component/CTransform.hpp"
#include "handle/MeshHandle.hpp"
#include "ecs/registry/Registry.hpp"
#include "scene/SceneManager.hpp"
#include "ecs/system/RenderSystem.hpp"
#include "ecs/component/CTransformCache.hpp"
#include "ecs/system/TransformSystem.hpp"
#include "engine/Window.hpp"
#include "engine/Engine.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
unsigned int viewport_width = 800;
unsigned int viewport_height = 600;

double deltaTime = 0.0; // time between current frame and last frame
double lastFrame = 0.0;

void run()
{
    Engine engine(viewport_width, viewport_height, "Game Engine");
    // Window &window = engine.getWindow();
    Scene &scene = engine.getSceneManager().GetCurrentScene();
    // build and compile our shader program
    // ------------------------------------
    ShaderHandle ourShader("assets/shaders/basic.vs", "assets/shaders/basic.fs"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    // vertices of a rectangle made from two triangles with uv coordinates
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

    scene.getRegistry().registerComponent<CTransform>();
    scene.getRegistry().registerComponent<CTransformCache>();
    scene.getRegistry().registerComponent<CMeshRenderer>();

    // auto rendererComponent = scene.getRegistry().createComponent<MeshRendererComponent>();
    // rendererComponent.setMesh();

    std::shared_ptr<MeshHandle> rendererMesh = std::make_shared<MeshHandle>(vertices, sizeof(vertices) / sizeof(float));

    Entity cubes[10];
    for (int i = 0; i < 10; i++)
    {
        Entity cube = scene.getRegistry().createEntity();
        cubes[i] = cube;

        auto &transform = scene.getRegistry().createComponent<CTransform>(cube);
        transform.position = cubePositions[i];

        CMeshRenderer &meshRenderer = scene.getRegistry().createComponent<CMeshRenderer>(cube);
        meshRenderer.mesh = rendererMesh;
        meshRenderer.shader = std::make_shared<ShaderHandle>(ourShader);
    }

    // Entity cubes[10];
    // for (int i = 0; i < 10; i++)
    // {
    //     cubes[i].getComponent<TransformComponent>().position = cubePositions[i];
    //     cubes[i].addComponent<MeshRendererComponent>()->setMesh(std::make_shared<MeshHandle>(vertices, sizeof(vertices) / sizeof(float)));
    // }

    // // texture
    TextureHandle texture("assets/textures/container.jpg");
    texture.filteringParameters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    scene.getMainCamera().setPosition(glm::vec3(0.0f, 0.0f, -3.0f));

    scene.getSystemScheduler().registerSystem<RenderSystem>();
    scene.getSystemScheduler().registerSystem<TransformSystem>();

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        texture.bind();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)viewport_width / (float)viewport_height, 0.1f, 100.0f);

        // render
        ourShader.use();
        ourShader.setMat4("view", scene.getMainCamera().getViewMatrix());
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("model", model);

        scene.getSystemScheduler().updateSystems((float)deltaTime);

        // glBindVertexArray(VAO);
        // for (int i = 0; i < 10; i++)
        // {
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, cubePositions[i]);
        //     float angle = 20.0f * i;
        //     model = glm::rotate(model, (float)glfwGetTime() + glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //     ourShader.setMat4("model", model);
        //     // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }

        // std::cout << "Number of TransformComponents: " << transforms.size() << std::endl;

        for (auto &cube : cubes)
        {
            auto &transform = scene.getRegistry().getComponent<CTransform>(cube);
            // auto &meshRenderer = scene.getRegistry().getComponent<CMeshRenderer>(cube);

            // std::cout << "Rendering entity with position: (" << transform->position.x << ", " << transform->position.y << ", " << transform->position.z << ")" << std::endl;
            transform.rotation.y += 20.0f * (float)deltaTime;
            transform.dirty = true;
            // ourShader.setMat4("model", transform.getModelMatrix());
            // meshRenderer.render();
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    // engine.run();

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

int main()
{
    try
    {
        run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

glm::vec2 lastMousePos = glm::vec2(viewport_width / 2.0f, viewport_height / 2.0f);
glm::vec2 mouseVelocity = glm::vec2(0.0f);

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // camera controls
    // --------------------------------------------
    // camera position
    Camera &mainCamera = SceneManager::GetCurrentScene().getMainCamera();
    glm::vec3 cameraVelocity = glm::vec3(0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraVelocity.z += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraVelocity.z -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraVelocity.x += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraVelocity.x -= 1.0f;

    if (glm::length(cameraVelocity) != 0)
        cameraVelocity = glm::normalize(cameraVelocity) * (float)deltaTime;
    mainCamera.setPosition(mainCamera.getPosition() + cameraVelocity);

    // camera orientation with mouse
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    glm::vec2 currentMousePos = glm::vec2(mouseX, mouseY);
    mouseVelocity = (currentMousePos - lastMousePos) * 0.1f;
    lastMousePos = currentMousePos;
    mainCamera.setRotation(mainCamera.getRotation() + glm::vec3(mouseVelocity.y, mouseVelocity.x, 0.0f));
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    viewport_width = width;
    viewport_height = height;

    window; // Unused parameter
}