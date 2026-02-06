#include "engine/Engine.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "ui/EditorUI.hpp"

#include "rendering/Renderer.hpp"
#include <DefaultBundle.hpp>
#include "systems/DefaultWorld.hpp"

#include <ui/EntityPanel.hpp>
#include <ui/ViewportPanel.hpp>
#include <ui/InspectorPanel.hpp>

#include <chrono>

using namespace engine_editor;
using namespace default_bundle;

int main()
{
    glfwInit();

    const char *glsl_version = "#version 330";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS safe
#endif

    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
    GLFWwindow *window = glfwCreateWindow((int)(1280 * main_scale), (int)(800 * main_scale), "ECS Game Engine", nullptr, nullptr);
    if (window == nullptr)
        return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale; // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    World *world;
    SystemRegistry *systems;
    ServiceRegistry *services;
    engine::Engine::InitializeEmbedded(world, systems, services)
        .addServices<engine::Renderer>()
        .addBundle<default_bundle::DefaultBundle>()
        .addSystems<DefaultWorld>();

    services->get<default_bundle::ShadowMapping>()->setEnabled(false); // Bug with shadow mapping in editor, disable for now

    using clock = std::chrono::steady_clock;
    auto lastFrame = clock::now();

    float deltaTime = 0.0f;

    systems->start();

    EntityPanel entityPanel(*world);
    ViewportPanel viewportPanel;
    InspectorPanel inspectorPanel(*world, *services);

    bool firstFrame = true;
    ImGuiID dockspaceID = 0;

    while (!glfwWindowShouldClose(window))
    {

        auto frameStart = clock::now();
        std::chrono::duration<double> delta = frameStart - lastFrame;
        lastFrame = frameStart;

        deltaTime = static_cast<float>(delta.count());

        glfwPollEvents();

        ImVec2 viewportSize = viewportPanel.getViewportSize();
        ViewportRenderer::Resize(viewportSize.x, viewportSize.y);
        ViewportRenderer::Begin();
        systems->update(deltaTime);
        ViewportRenderer::End();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        // ----- Create Dockspace -----
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                        ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        dockspaceID = ImGui::GetID("MainDockspace");
        ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        // Setup initial docking layout on first frame
        if (firstFrame)
        {
            firstFrame = false;
            ImGui::DockBuilderRemoveNode(dockspaceID);
            ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->WorkSize);

            ImGuiID dock_main_id = dockspaceID;
            ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
            ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, nullptr, &dock_main_id);

            ImGui::DockBuilderDockWindow("Viewport", dock_main_id);
            ImGui::DockBuilderDockWindow("Entities", dock_left_id);
            ImGui::DockBuilderDockWindow("Inspector", dock_right_id);

            ImGui::DockBuilderFinish(dockspaceID);
        }

        ImGui::End();

        // ----- Your ImGui UI -----
        entityPanel.draw();
        viewportPanel.draw(ViewportRenderer::GetFramebufferTexture());
        inspectorPanel.draw(entityPanel.getSelectedEntity());
        // -------------------------

        // // Render
        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
