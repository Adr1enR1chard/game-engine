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
file(GLOB_RECURSE DEFAULT_RENDERING_SOURCES
    bundle/default-rendering/src/*.cpp
)

add_library(default_rendering
    ${DEFAULT_RENDERING_SOURCES}
)
target_include_directories(default_rendering PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/bundle/default-rendering/include
)
target_link_libraries(default_rendering PUBLIC
    assets_format
)

# Library: GLFW platform
add_library(default_rendering_glfw
    bundle/default-rendering/src/service/ShadowMapping.cpp
)
target_include_directories(default_rendering_glfw PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/bundle/default-rendering/include
)

target_link_libraries(default_rendering_glfw PUBLIC
    glfw
    OpenGL::GL
    assets_loader
)
if (WIN32)
    target_link_libraries(default_rendering_glfw PUBLIC opengl32)
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
target_link_libraries(engine_core
PUBLIC
    glfw
    OpenGL::GL
    assets_format
    assets_loader
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

#Library: ImGui
file(GLOB_RECURSE IMGUI_SOURCES
    tools/editor/include/imgui/*.cpp
)
add_library(imgui
    ${IMGUI_SOURCES}
)
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/editor/include/imgui
)
target_link_libraries(imgui 
PUBLIC 
    glfw
    OpenGL::GL
)

#Library : Editor
file(GLOB_RECURSE EDITOR_SOURCES
    tools/editor/src/*.cpp
)
add_library(engine_editor
    ${EDITOR_SOURCES}
)
target_include_directories(engine_editor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/editor/include
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/editor/src
)
target_link_libraries(engine_editor PUBLIC
    engine_core
    engine_vendor
    imgui
    default_rendering
    default_rendering_glfw
)

###########################
## Executables
###########################

set(EXAMPLES_OUTPUT_DIR ${CMAKE_BINARY_DIR}/examples)
set(TOOLS_OUTPUT_DIR ${CMAKE_BINARY_DIR}/tools)

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
    default_rendering
    engine_core
    engine_vendor
    default_rendering_glfw
)
enable_strict_warnings(BasicScene)

set_target_properties(BasicScene PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${EXAMPLES_OUTPUT_DIR}/BasicScene"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${EXAMPLES_OUTPUT_DIR}/BasicScene"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${EXAMPLES_OUTPUT_DIR}/BasicScene"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXAMPLES_OUTPUT_DIR}/BasicScene"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${EXAMPLES_OUTPUT_DIR}/BasicScene"
)

#Executable: Space Game
file(GLOB_RECURSE SPACE_GAME_SOURCES
    examples/space_game/src/*.cpp
)

add_executable(SpaceGame
    examples/space_game/main.cpp
    ${SPACE_GAME_SOURCES}
)

target_include_directories(SpaceGame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/space_game/include
)

target_link_libraries(SpaceGame
PUBLIC
    default_rendering
    default_rendering_glfw
    engine_core
    engine_vendor
)

set_target_properties(SpaceGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${EXAMPLES_OUTPUT_DIR}/SpaceGame"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${EXAMPLES_OUTPUT_DIR}/SpaceGame"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${EXAMPLES_OUTPUT_DIR}/SpaceGame"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${EXAMPLES_OUTPUT_DIR}/SpaceGame"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${EXAMPLES_OUTPUT_DIR}/SpaceGame"
)

# Executable: Editor
add_executable(Editor
    tools/editor/src/main.cpp
)
target_link_libraries(Editor PUBLIC
    engine_editor
)
set_target_properties(Editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${TOOLS_OUTPUT_DIR}/Editor"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${TOOLS_OUTPUT_DIR}/Editor"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${TOOLS_OUTPUT_DIR}/Editor"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${TOOLS_OUTPUT_DIR}/Editor"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${TOOLS_OUTPUT_DIR}/Editor"
)

###########################
## Commands and Targets
###########################

# Command: Assets compilation
set(ASSET_COMPILER_EXE "$<TARGET_FILE:assets_compiler>")
set(DEFAULT_BUNDLE_ASSETS_SRC "${CMAKE_SOURCE_DIR}/bundle/default/default-bundle-assets")
set(EXTENSIONS jpg png bmp tga gif  obj fbx gltf vert frag)

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

include(AssetsCompilation)

add_asset_pipeline(
    TARGET       BasicScene
    SOURCE_DIR   "${CMAKE_SOURCE_DIR}/examples/basic_scene/assets"
    RUNTIME_DIR  "$<TARGET_FILE_DIR:BasicScene>/assets"
    COMPILER     "${ASSET_COMPILER_EXE}"
    EXTENSIONS   ${EXTENSIONS}
)

add_asset_pipeline(
    TARGET       BasicScene
    ALIAS        core_bundle
    SOURCE_DIR   "${DEFAULT_BUNDLE_ASSETS_SRC}"
    RUNTIME_DIR  "$<TARGET_FILE_DIR:BasicScene>/default-bundle-assets"
    COMPILER     "${ASSET_COMPILER_EXE}"
    EXTENSIONS   ${EXTENSIONS}
)

add_asset_pipeline(
    TARGET       SpaceGame
    SOURCE_DIR   "${CMAKE_SOURCE_DIR}/examples/space_game/assets"
    RUNTIME_DIR  "$<TARGET_FILE_DIR:SpaceGame>/assets"
    COMPILER     "${ASSET_COMPILER_EXE}"
    EXTENSIONS   ${EXTENSIONS}
)

add_asset_pipeline(
    TARGET       SpaceGame
    ALIAS        core_bundle
    SOURCE_DIR   "${DEFAULT_BUNDLE_ASSETS_SRC}"
    RUNTIME_DIR  "$<TARGET_FILE_DIR:SpaceGame>/default-bundle-assets"
    COMPILER     "${ASSET_COMPILER_EXE}"
    EXTENSIONS   ${EXTENSIONS}
)

add_asset_pipeline(
    TARGET       Editor
    ALIAS        core_bundle
    SOURCE_DIR   "${DEFAULT_BUNDLE_ASSETS_SRC}"
    RUNTIME_DIR  "$<TARGET_FILE_DIR:Editor>/default-bundle-assets"
    COMPILER     "${ASSET_COMPILER_EXE}"
    EXTENSIONS   ${EXTENSIONS}
)
