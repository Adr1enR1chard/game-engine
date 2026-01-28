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
add_library(default_bundle
    bundle/default/src/system/RenderSystem.cpp
    bundle/default/src/system/CameraSystem.cpp
    bundle/default/src/system/TransformSystem.cpp
    bundle/default/src/system/LightSystem.cpp
    bundle/default/src/system/LightSystem.cpp
    bundle/default/src/system/PlatformSystem.cpp
    bundle/default/src/system/EnvironmentSystem.cpp
    bundle/default/src/service/Input.cpp
    bundle/default/src/utils/AssetsLoader.cpp
)
target_include_directories(default_bundle PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/bundle/default/include
)

# Library: GLFW platform
add_library(default_bundle_glfw
    bundle/default/src/platform/glfw/service/Window.cpp
    bundle/default/src/platform/glfw/service/resource/MeshResource.cpp
    bundle/default/src/platform/glfw/service/resource/ShaderResource.cpp
    bundle/default/src/platform/glfw/service/resource/MaterialResource.cpp
    bundle/default/src/platform/glfw/service/resource/TextureResource.cpp
    bundle/default/src/platform/glfw/service/resource/ModelResource.cpp
    bundle/default/src/platform/glfw/service/factory/MaterialFactory.cpp
    bundle/default/src/platform/glfw/service/factory/ShaderFactory.cpp
    bundle/default/src/platform/glfw/service/factory/ModelFactory.cpp
    bundle/default/src/platform/glfw/service/factory/MeshFactory.cpp
    bundle/default/src/platform/glfw/service/factory/TextureFactory.cpp
    bundle/default/src/DefaultBundle.cpp
)
target_include_directories(default_bundle_glfw PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/bundle/default/include
)

# Library: Rendering
add_library(engine_core
    engine/src/Engine.cpp
    engine/src/registry/World.cpp
    engine/src/utils/key_mapping.cpp
    engine/src/utils/Log.cpp
    engine/src/model/Service.cpp
)
target_include_directories(engine_core PUBLIC
     ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
)
target_compile_definitions(engine_core PUBLIC GLFW_INCLUDE_NONE)

# Library: third-party single-file implementations (GLAD, stb_image)
add_library(engine_vendor
    engine/src/third_party/glad/glad.c
    engine/src/third_party/stb_image/stb_image.cpp
)
target_include_directories(engine_vendor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
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

# Asset compiler
include (FetchContent)
FetchContent_Declare(
    asset_compiler
    GIT_REPOSITORY https://github.com/Adr1enR1chard/assets-compiler.git
    GIT_TAG main
)
FetchContent_MakeAvailable(asset_compiler)

# Compile asset compiler tool from git repository
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/assets.bin
    COMMAND $<TARGET_FILE:AssetCompiler>
            ${CMAKE_SOURCE_DIR}/assets
            ${CMAKE_CURRENT_BINARY_DIR}/assets.bin
    DEPENDS AssetCompiler
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Compiling assets"
)

add_custom_target(compile_assets
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/assets.bin
)

# Link third-party to engine libraries
target_link_libraries(default_bundle_glfw PUBLIC
    glfw
    OpenGL::GL
)
target_link_libraries(engine_core PUBLIC
    assimp::assimp
)
if (WIN32)
    target_link_libraries(default_bundle_glfw PUBLIC opengl32)
endif()

# Executable: BasicScene sample
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

set(ASSET_COMPILER_EXE "$<TARGET_FILE:AssetCompiler>")
set(ASSETS_SRC "${CMAKE_SOURCE_DIR}/examples/basic_scene/assets")
set(DEFAULT_BUNDLE_ASSETS_SRC "${CMAKE_SOURCE_DIR}/bundle/default/default-bundle-assets")

add_custom_command(TARGET BasicScene POST_BUILD
    COMMAND ${ASSET_COMPILER_EXE} "${ASSETS_SRC}" "$<TARGET_FILE_DIR:BasicScene>/assets"
    COMMAND ${ASSET_COMPILER_EXE} "${DEFAULT_BUNDLE_ASSETS_SRC}" "$<TARGET_FILE_DIR:BasicScene>/default-bundle-assets"
    COMMENT "Running AssetCompiler to process assets for BasicScene..."
)

add_custom_command(TARGET BasicScene POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:BasicScene>/assets"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${ASSETS_SRC}"
          "$<TARGET_FILE_DIR:BasicScene>/assets"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${DEFAULT_BUNDLE_ASSETS_SRC}"
          "$<TARGET_FILE_DIR:BasicScene>/default-bundle-assets")

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


enable_strict_warnings(engine_core)
enable_strict_warnings(engine_vendor)
enable_strict_warnings(default_bundle_glfw)
enable_strict_warnings(BasicScene)
# enable_strict_warnings(RegistryTests)
# enable_strict_warnings(SystemTests)