# Game Engine

A lightweight, modular C++20 game engine built around an Entity-Component-System (ECS) architecture with OpenGL rendering.

## Project Description

This game engine is designed to be minimalistic and extensible, providing the essential infrastructure for 3D game development while keeping the core lightweight. The engine provides:

- **ECS-based architecture** for efficient entity and component management
- **OpenGL rendering pipeline** with PBR (Physically Based Rendering) material system
- **GLFW-based window management** and input handling
- **Asset loading** supporting GLTF models and various texture formats
- **Modular bundle system** for organizing and reusing functionality
- **Transform hierarchy** with automatic matrix computation
- **Camera system** with view/projection matrix management
- **Lighting system** supporting directional and point lights

The engine leverages modern C++ features (C++20) and industry-standard libraries including GLFW, GLM, Assimp, and GLAD.

## Project Philosophy

The engine's core is intentionally minimal, consisting of just two fundamental components:

1. **ECS Registry (World)** - A type-safe, efficient storage system for entities and components
2. **Game Loop** - A fixed-structure loop handling input, update, and render phases

All additional functionality is provided through **Bundles** - modular packages that add systems, services, and components to the engine. This design allows:

- **Modularity**: Add only the features you need
- **Extensibility**: Create custom bundles for specific game requirements
- **Reusability**: Share bundles across different projects
- **Clarity**: Clear separation between engine core and feature implementations

The `DefaultBundle` provides essential 3D rendering capabilities, but users can create specialized bundles for physics, audio, networking, or any other game-specific features.

## Project Architecture

### Core Structure

```
GameEngine/
├── engine/                    # Engine core
│   ├── include/
│   │   ├── Engine.hpp        # Main engine class
│   │   ├── registry/         # ECS registries
│   │   │   ├── World.hpp            # Entity-Component storage
│   │   │   ├── SystemRegistry.hpp   # System management
│   │   │   ├── ServiceRegistry.hpp  # Service container
│   │   │   └── ComponentStorage.hpp # Component storage implementation
│   │   ├── model/            # Core abstractions
│   │   │   ├── Entity.hpp           # Entity type definition
│   │   │   ├── System.hpp           # System base class
│   │   │   ├── Service.hpp          # Service base class
│   │   │   ├── Bundle.hpp           # Bundle base class
│   │   │   └── ...                  # Model, Mesh, Material, Texture
│   │   └── utils/            # Utilities
│   └── src/                  # Implementation files
│
├── bundle/                    # Bundle implementations
│   └── default/              # Default rendering bundle
│       ├── include/
│       │   ├── DefaultBundle.hpp
│       │   ├── component/    # Rendering components
│       │   │   ├── CTransform.hpp
│       │   │   ├── CCamera.hpp
│       │   │   ├── CMeshRenderer.hpp
│       │   │   ├── CPointLight.hpp
│       │   │   └── CDirectionalLight.hpp
│       │   ├── system/       # Rendering systems
│       │   │   ├── TransformSystem.hpp
│       │   │   ├── CameraSystem.hpp
│       │   │   ├── RenderSystem.hpp
│       │   │   └── LightSystem.hpp
│       │   └── service/      # Platform services
│       │       ├── Window.hpp
│       │       └── Input.hpp
│       └── src/              # Implementation files
│
└── examples/                  # Example projects
    └── basic_scene/          # Basic 3D scene example
        ├── main.cpp
        └── assets/           # Textures and models
```

### Key Concepts

#### **World (ECS Registry)**
The `World` class manages all entities and components. It provides:
- `create<Components...>()` - Create entities with components
- `get<Components...>()` - Query entities by component types
- `getFrom<Components...>(entity)` - Get components from specific entity
- `add<Components...>(entity)` - Add components to existing entity

#### **Systems**
Systems implement game logic and operate on entities with specific component combinations:
- `start()` - Called once at startup
- `input()` - Handle input events
- `update()` - Update game logic
- `render()` - Perform rendering

#### **Services**
Services are singletons providing engine-wide functionality (Window, Input, etc.)

#### **Bundles**
Bundles package related systems, services, and components together:
```cpp
class MyBundle : public Bundle {
    void apply(Engine& engine) const override {
        engine.addSystems<MySystem1, MySystem2>();
        engine.addServices<MyService>();
    }
};
```

### Game Loop

The engine's game loop follows a fixed structure:
```
1. Poll Events (Input)
2. Input Phase (Systems process input)
3. Update Phase (Game logic updates)
4. Render Phase (Draw scene)
5. Swap Buffers (Present frame)
```

Each system receives the `World`, `ServiceRegistry`, and `deltaTime` during each phase.

## Basic Example

Here's a minimal example creating a 3D scene with a camera, lights, and a rotating 3D model:

```cpp
#include <DefaultBundle.hpp>

// Custom system to rotate a model
class BackPackSystem : public System
{
    Entity backpackEntity;
    
    void start(World& world, ServiceRegistry& /*services*/) override
    {
        // Create an entity with a 3D model
        backpackEntity = world.create(
            CTransform{
                .position = glm::vec3(0.0f, 0, -6.0f), 
                .scale = glm::vec3(0.01f)
            },
            CModelRenderer{
                .model = Model("assets/models/backpack.gltf"),
                .materialOverrides = std::vector<MaterialInstance>{
                    MaterialInstance::Default({
                        .albedoMap    = Texture("assets/textures/backpack/albedo.jpeg"),
                        .metallicMap  = Texture("assets/textures/backpack/metallic.png"),
                        .roughnessMap = Texture("assets/textures/backpack/roughness.png"),
                        .normalMap    = Texture("assets/textures/backpack/normal.png"),
                    })
                },
            }
        );
    }

    void update(World& world, ServiceRegistry& /*services*/, double deltaTime) override
    {
        // Rotate the model over time
        if (auto [entity, transform] = world.getFrom<CTransform>(backpackEntity); entity != 0) {
            transform->rotation.y += 100.0f * static_cast<float>(deltaTime);
        }
    }
};

// System to set up the initial scene
class StartupSystem : public System
{
    void start(World& world, ServiceRegistry& services) override
    {
        // Set background color
        services.get<Window>()->setClearColor(glm::vec3(0.1f, 0.1f, 0.1f));
        
        // Create camera
        world.create(
            CCamera{}, 
            CTransform{.position = glm::vec3(0.0f, 0.0f, 3.0f)}
        );
        
        // Create directional light (sun)
        world.create(
            CDirectionalLight{
                .direction = glm::vec3(-0.2f, -1.0f, -0.3f),
                .color     = glm::vec3(1.0f, 1.0f, 1.0f),
                .ambient   = 0.2f,
                .intensity = 0.0f
            }
        );
        
        // Create point light
        world.create(
            CPointLight{
                .color = glm::vec3(1.0f, 1.0f, 1.0f), 
                .intensity = 500.0f
            },
            CTransform{.position = glm::vec3(-2.0f, 2.0f, -2.0f)},
            CMeshRenderer{
                .mesh = Mesh::Cube(),
                .material = MaterialInstance::Default({
                    .albedo    = glm::vec3(1.0f, 1.0f, 1.0f),
                    .metallic  = 0.0f,
                    .roughness = 1.0f,
                })
            }
        );
        
        // Create textured wall
        world.create(
            CMeshRenderer{
                .mesh = Mesh::Cube(),
                .material = MaterialInstance::Default({
                    .albedoMap    = Texture("assets/textures/wall/diffuse.png"),
                    .roughnessMap = Texture("assets/textures/wall/roughness.png"),
                    .normalMap    = Texture("assets/textures/wall/normal.png")
                })
            },
            CTransform{
                .position = glm::vec3(0.0f, 0.0f, -10.0f), 
                .scale = glm::vec3(20.0f, 20.0f, 1.0f)
            }
        );
    }
};

int main()
{
    Engine::Init()
        .addBundle<DefaultBundle>() // Add the default rendering bundle
        .addSystems<StartupSystem, BackPackSystem>() // Add custom game systems
        .run<Window>(1280, 720, "Basic Scene Example", false); // Run the engine

    return 0;
}
```

### Key Takeaways

1. **Include the DefaultBundle** to get rendering capabilities
2. **Create systems** by inheriting from `System` and implementing lifecycle methods
3. **Use the World** to create entities with components
4. **Add systems to the engine** before running
5. **Start the game loop** with `engine.run<WindowType>()`

## Building

Requirements:
- CMake 3.20+
- C++20 compatible compiler
- OpenGL 3.3+

```bash
# Configure
cmake -B build

# Build
cmake --build build --config Debug

# Run example
./build/bin/Debug/BasicScene
```

## Dependencies

All dependencies are automatically fetched via CMake FetchContent:
- **GLFW** - Window and input handling
- **GLM** - Mathematics library
- **Assimp** - 3D model loading
- **GLAD** - OpenGL function loader
- **stb_image** - Image loading

## License

MIT License. See LICENSE file for details.
