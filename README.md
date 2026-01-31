# Game Engine

A lightweight, modular C++20 game engine built around an Entity-Component-System (ECS) architecture with OpenGL rendering.

<img width="1919" height="1079" alt="image" src="https://github.com/user-attachments/assets/d166ca30-8bbb-464b-9f04-7bb72550f1ff" />

## Project Description

This game engine is intentionally minimal and highly extensible. It supplies only the foundational infrastructure required for 3D game development, keeping the core lightweight by design. The engine itself is limited to an ECS framework and a runtime scheduler, while rendering functionality is delegated to an external bundle.

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
