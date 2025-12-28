# C++ OpenGL Game Engine

A small, cross‑platform game engine written in C++ using OpenGL (3.3+), GLFW for windowing/input, and GLM for math. Built with CMake.

## Features
- GLFW window + OpenGL context
- Keyboard/mouse input
- Camera + transforms via GLM
- Basic renderer and shader management
- Game loop and scene update hooks

## Requirements
- C++17 compiler (MSVC, Clang, or GCC)
- CMake ≥ 3.20
- OpenGL 3.3+ capable GPU/driver
- Dependencies: GLFW, GLM, and an OpenGL function loader (e.g., GLAD or GLEW)

## Build
- Unix-like:
  - `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`
  - `cmake --build build --config Release`
- Windows (MSVC):
  - `cmake -S . -B build -G "Visual Studio 17 2022"`
  - `cmake --build build --config Release`

## Run
- The executable is generated under `build/` (MSVC: `build/Release/`).
- Run from the project root to ensure relative asset/shader paths resolve:
  - Unix: `./build/engine` (or your target name)
  - Windows: `build/Release/engine.exe`

## Project structure
- `src/` — engine sources
- `include/` — public headers
- `assets/` — game assets (textures, models, shaders)
- `build/` — out-of-source build directory

## License
MIT (or choose your preferred license).
