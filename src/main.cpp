#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "handle/ShaderHandle.hpp"
#include "handle/TextureHandle.hpp"
#include "ecs/component/MeshRendererComponent.hpp"
#include "ecs/component/TransformComponent.hpp"
#include "handle/MeshHandle.hpp"
#include "ecs/registry/Registry.hpp"
#include "scene/SceneManager.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
unsigned int viewport_width = 800;
unsigned int viewport_height = 600;

float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(viewport_width, viewport_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

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

    Scene &scene = SceneManager::GetCurrentScene();

    auto rendererComponent = scene.getWorld().createComponent<MeshRendererComponent>();
    rendererComponent.setMesh(std::make_shared<MeshHandle>(vertices, sizeof(vertices) / sizeof(float)));

    for (int i = 0; i < 10; i++)
    {
        Entity cube = scene.getWorld().createEntity();

        auto &transform = scene.getWorld().createComponent<TransformComponent>();
        transform.bindToEntity(cube);
        transform.position = cubePositions[i];

        rendererComponent.bindToEntity(cube);
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

    glEnable(GL_DEPTH_TEST);

    scene.getMainCamera().setPosition(glm::vec3(0.0f, 0.0f, -3.0f));

    // render loop
    // -----------
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

        auto transforms = scene.getWorld().getComponents<TransformComponent>();

        // std::cout << "Number of TransformComponents: " << transforms.size() << std::endl;

        for (auto &transform : transforms)
        {
            transform->rotation.y += 20.0f * deltaTime;
            // std::cout << "Rendering entity with position: (" << transform->position.x << ", " << transform->position.y << ", " << transform->position.z << ")" << std::endl;
            ourShader.setMat4("model", transform->getModelMatrix());
            rendererComponent.render();
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
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
        cameraVelocity = glm::normalize(cameraVelocity) * deltaTime;
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
}