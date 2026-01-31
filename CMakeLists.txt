cmake_minimum_required(VERSION 3.20)

# Project
project(BasicScene LANGUAGES C CXX)

# Warnings configuration
include(cmake/CompilerWarnings.cmake)

# Standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Output directories (useful for Visual Studio multi-config)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable CTest for unit tests
enable_testing()

###########################
## Dependencies
###########################

#### GLFW
include(FetchContent)
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.4
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

#### OpenGL
find_package(OpenGL REQUIRED)

#### Assets Compiler
include (FetchContent)
FetchContent_Declare(
    assets_compiler
    GIT_REPOSITORY https://github.com/Adr1enR1chard/assets-compiler.git
    GIT_TAG main
)
FetchContent_MakeAvailable(assets_compiler)

###########################
## Libraries
###########################

# Library: Default bundle
file(GLOB_RECURSE DEFAULT_BUNDLE_SYSTEMS
    bundle/default/src/system/*.cpp
)

add_library(default_bundle
    ${DEFAULT_BUNDLE_SYSTEMS}
    bundle/default/src/DefaultBundle.cpp
)
target_include_directories(default_bundle PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/bundle/default/include
)
target_link_libraries(default_bundle PUBLIC
    assets_format
)
enable_strict_warnings(default_bundle)

# Library: GLFW platform
add_library(default_bundle_glfw
    bundle/default/src/platform/glfw/service/factory/MaterialFactory.cpp
    bundle/default/src/platform/glfw/service/factory/ShaderFactory.cpp
    bundle/default/src/platform/glfw/service/factory/ModelFactory.cpp
    bundle/default/src/platform/glfw/service/factory/MeshFactory.cpp
    bundle/default/src/platform/glfw/service/factory/TextureFactory.cpp
)
target_include_directories(default_bundle_glfw PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/bundle/default/include
)
enable_strict_warnings(default_bundle_glfw)

target_link_libraries(default_bundle_glfw PUBLIC
    glfw
    OpenGL::GL
    assets_loader
)
if (WIN32)
    target_link_libraries(default_bundle_glfw PUBLIC opengl32)
endif()

# Library: Rendering
file(GLOB_RECURSE ENGINE_SOURCES
    engine/src/*.cpp
)

add_library(engine_core
    ${ENGINE_SOURCES}
)
target_include_directories(engine_core PUBLIC
     ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
)
target_compile_definitions(engine_core PUBLIC GLFW_INCLUDE_NONE)
target_link_libraries(engine_core
PUBLIC
    assets_format
)
enable_strict_warnings(engine_core)

# Library: third-party single-file implementations (GLAD, stb_image)
add_library(engine_vendor
    engine/src/third_party/glad/glad.c
)
target_include_directories(engine_vendor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
enable_strict_warnings(engine_vendor)

###########################
## Executables
###########################

# Executable: BasicScene
add_executable(BasicScene
    examples/basic_scene/main.cpp
)
target_include_directories(BasicScene PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/basic_scene
)
target_link_libraries(BasicScene PRIVATE
    default_bundle
    engine_core
    engine_vendor
    default_bundle_glfw
)
enable_strict_warnings(BasicScene)

###########################
## Commands and Targets
###########################

# Command: Assets compilation
set(ASSET_COMPILER_EXE "$<TARGET_FILE:assets_compiler>")
set(BASIC_SCENE_ASSETS_SRC "${CMAKE_SOURCE_DIR}/examples/basic_scene/assets")
set(BASIC_SCENE_ASSETS_RUNTIME "$<TARGET_FILE_DIR:BasicScene>/assets")
set(DEFAULT_BUNDLE_ASSETS_SRC "${CMAKE_SOURCE_DIR}/bundle/default/default-bundle-assets")
set(DEFAULT_BUNDLE_ASSETS_RUNTIME "$<TARGET_FILE_DIR:BasicScene>/default-bundle-assets")
set(EXTENSIONS jpg png bmp tga gif  obj fbx gltf vert frag)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

include(AssetsCompilation)

add_asset_pipeline(
    TARGET       BasicScene
    SOURCE_DIR   "${BASIC_SCENE_ASSETS_SRC}"
    RUNTIME_DIR  "${BASIC_SCENE_ASSETS_RUNTIME}"
    COMPILER     "${ASSET_COMPILER_EXE}"
    EXTENSIONS   ${EXTENSIONS}
)

add_asset_pipeline(
    TARGET       default_bundle
    SOURCE_DIR   "${DEFAULT_BUNDLE_ASSETS_SRC}"
    RUNTIME_DIR  "${DEFAULT_BUNDLE_ASSETS_RUNTIME}"
    COMPILER     "${ASSET_COMPILER_EXE}"
    EXTENSIONS   ${EXTENSIONS}
)
