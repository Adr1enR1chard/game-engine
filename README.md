# C++ OpenGL Game Engine

Cross‑platform, modern C++ game engine powered by OpenGL 3.3+, designed around a lightweight ECS (Entity‑Component‑System) architecture. Uses GLFW for windowing/input, GLAD for OpenGL function loading, GLM for math, Assimp for model loading, and stb_image for textures. Built with CMake.

## Overview
- Focuses on clarity and composability: systems operate over components in a `World`, the renderer consumes `Mesh`/`Model` + `Material` data to draw.
- Batteries‑included example: [examples/basic_scene/main.cpp](examples/basic_scene/main.cpp) demonstrates cameras, lights, mesh rendering, model loading, and a simple animated light.

## Architecture
- **ECS Core**: `World`, `Entity`, `System`
  - Components: `CTransform`, `CCamera`, `CMeshRenderer`, `CModelRenderer`, `CPointLight`, `CDirectionalLight`, etc.
  - Systems: `TransformSystem`, `CameraSystem`, `RenderSystem`, `LightSystem`, plus user systems.
- **Rendering**:
  - `Mesh::Cube()` and imported `Model` via Assimp (GLTF, OBJ, etc.)
  - `Material` / `MaterialInstance` with optional texture maps (albedo, normal, etc.)
  - Forward renderer with basic lighting (directional + point)
- **Services**:
  - `Window` service (GLFW) for context creation, input, clear color, viewport
  - Asset loading: Assimp for models, stb_image for textures

## Features
- Window, input, and main loop management
- Camera (position/orientation) and transform hierarchy
- Static meshes and model loading
- Directional and point lights
- Simple material system with texture overrides
- Extensible ECS systems and phases (`Start`, `Update`)

## Requirements
- C++17 compiler (MSVC, Clang, or GCC)
- CMake ≥ 3.20
- OpenGL 3.3+ capable GPU/driver
- Third‑party libraries: GLFW, GLM, GLAD, Assimp, stb_image (vendored via CMake)

## Build (Windows/MSVC)
1. Configure a Visual Studio solution:
   ```bash
   cmake -S . -B build -G "Visual Studio 17 2022"
   ```
2. Build the example target:
   ```bash
   cmake --build build --target BasicScene --config Release
   ```

On Unix‑like systems:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Run the Example
- Assets for the example live under [examples/basic_scene/assets](examples/basic_scene/assets).
- Run the `BasicScene` binary with its working directory set to `examples/basic_scene/` or the project root so relative asset paths resolve.
- Typical MSVC output path: `build/bin/Release/BasicScene.exe` (Debug builds under `build/bin/Debug/`).

## Example: Basic Scene
Minimal example extracted from [examples/basic_scene/main.cpp](examples/basic_scene/main.cpp):

```cpp
#include <engine/Core.hpp>

class StartupSystem : public System {
  public:
    void start(World& world) override {
      // Clear color and camera
      world.Serv<Window>().setClearColor(glm::vec3(0.1f));
      world.create(CCamera{}, CTransform{.position = glm::vec3(0.0f, 0.0f, 3.0f)});

      // Lights
      world.create(CDirectionalLight{.direction = {-0.2f, -1.0f, -0.3f}});
      world.create(CPointLight{.color = {1.0f, 0.5f, 0.5f}, .intensity = 5.0f},
                   CTransform{.position = {-2.0f, 2.0f, -2.0f}, .scale = glm::vec3(0.2f)},
                   CMeshRenderer{ .mesh = Mesh::Cube(), .material = Material::Default() });

      // Model + material override (assets/ relative to working dir)
      world.create(CTransform{.position = {0.0f, 0.0f, -6.0f}, .scale = glm::vec3(0.01f)},
                   CModelRenderer{ .model = Model("assets/models/backpack.gltf"),
                     .materialOverrides = { MaterialInstance::Default({
                       .albedoMap = Texture("assets/textures/backpack/baseColor.jpeg")
                     }) } });
    }
};

int main() {
  Engine engine(800, 600, "Game Engine");
  engine.systems().registerSystem<TransformSystem>();
  engine.systems().registerSystem<CameraSystem>();
  engine.systems().registerSystem<RenderSystem>();
  engine.systems().registerSystem<LightSystem>();
  engine.systems().registerSystem<StartupSystem>(SystemPhase::Start);
  engine.run();
}
```

This shows how to:
- Configure clear color and spawn a `CCamera`
- Add a directional light and a point light with a cube mesh
- Load a GLTF model with a texture override
- Register core systems and run the engine

## Project Layout
- `src/` — engine sources (ECS, rendering, utilities)
- `include/` — public headers (engine API)
- `examples/basic_scene/` — runnable example with assets
- `renderdoc/` — capture files for debugging rendering
- `build/` — out‑of‑source build artifacts (solutions, targets)

## Extending
- Add components to describe state (e.g., `CPlayer`, `CVelocity`).
- Write systems that iterate over matching components and mutate state or render.
- Use `SystemPhase::Start` for one‑time scene setup, and default phase for per‑frame updates.

## Notes
- Ensure the working directory points to a folder where asset paths resolve (example uses `assets/...`).
- If you modify shaders or assets, prefer running from an IDE or setting the working directory accordingly.
