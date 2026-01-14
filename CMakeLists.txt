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

# Library: ECS systems
add_library(engine_ecs
    src/ecs/system/RenderSystem.cpp
    src/ecs/system/CameraSystem.cpp
    src/ecs/system/TransformSystem.cpp
    src/ecs/system/LightSystem.cpp
    src/ecs/World.cpp
    src/utils/Bundle.cpp
    src/render/Input.cpp
)
target_include_directories(engine_ecs PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Library: Rendering
add_library(engine_render
    src/render/Window.cpp
    src/render/Mesh.cpp
    src/render/Texture.cpp
    src/render/Material.cpp
    src/render/MaterialInstance.cpp
)
target_include_directories(engine_render PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_definitions(engine_render PUBLIC GLFW_INCLUDE_NONE)

# Library: Model loading
add_library(engine_model
    src/model/Model.cpp
)
target_include_directories(engine_model PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Library: third-party single-file implementations (GLAD, stb_image)
add_library(engine_vendor
    src/third_party/glad/glad.c
    src/third_party/stb_image/stb_image.cpp
)
target_include_directories(engine_vendor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Dependencies: GLFW via FetchContent
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

# OpenGL
find_package(OpenGL REQUIRED)

# Assimp
include(FetchContent)
FetchContent_Declare(assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG master
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(assimp)

# Link third-party to engine libraries
target_link_libraries(engine_render PUBLIC
    glfw
    OpenGL::GL
)
target_link_libraries(engine_model PUBLIC
    assimp::assimp
)
if (WIN32)
    target_link_libraries(engine_render PUBLIC opengl32)
endif()

# Executable: BasicScene sample
add_executable(BasicScene
    examples/basic_scene/main.cpp
)
target_include_directories(BasicScene PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(BasicScene PRIVATE
    engine_ecs
    engine_render
    engine_model
    engine_vendor
)

## Note: dependencies are linked to engine_* libraries above

add_custom_command(TARGET BasicScene POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:BasicScene>/assets"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/examples/basic_scene/assets"
          "$<TARGET_FILE_DIR:BasicScene>/assets")

# # Registry unit tests
# add_executable(RegistryTests
#     tests/RegistryTests.cpp
# )
# target_include_directories(RegistryTests PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/src
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )
# target_link_libraries(RegistryTests PRIVATE
#     engine_ecs
# )
# add_test(NAME RegistryTests COMMAND RegistryTests)

# # SystemScheduler unit tests
# add_executable(SystemTests
#     tests/SystemTests.cpp
# )
# target_include_directories(SystemTests PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/src
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )
# target_link_libraries(SystemTests PRIVATE
#     engine_ecs
# )
# add_test(NAME SystemTests COMMAND SystemTests)


enable_strict_warnings(engine_ecs)
enable_strict_warnings(engine_render)
enable_strict_warnings(engine_model)
enable_strict_warnings(engine_vendor)
enable_strict_warnings(BasicScene)
# enable_strict_warnings(RegistryTests)
# enable_strict_warnings(SystemTests)