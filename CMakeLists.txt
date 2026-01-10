cmake_minimum_required(VERSION 3.20)

# Project
project(BasicScene LANGUAGES C CXX)

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

# Sources
add_executable(BasicScene
	examples/basic_scene/main.cpp
	src/ecs/system/RenderSystem.cpp
    src/ecs/system/CameraSystem.cpp
	src/ecs/system/SystemScheduler.cpp
    src/ecs/system/TransformSystem.cpp
    src/ecs/system/LightSystem.cpp
    src/render/Window.cpp
    src/render/Shader.cpp
    src/render/Mesh.cpp
    src/render/Texture.cpp
    src/render/Material.cpp
	src/third_party/glad/glad.c
	src/third_party/stb_image/stb_image.cpp
    src/model/Model.cpp
)

# Include directories for local headers (GLAD, etc.)
target_include_directories(BasicScene PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Avoid GLFW including GL headers; we provide them via GLAD
target_compile_definitions(BasicScene PRIVATE GLFW_INCLUDE_NONE)

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

# Link libraries
target_link_libraries(BasicScene PRIVATE
		glfw
		OpenGL::GL
        assimp::assimp
)

# Windows-specific (ensure opengl32 linkage where necessary)
if (WIN32)
	target_link_libraries(BasicScene PRIVATE opengl32)
endif()

add_custom_command(TARGET BasicScene POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:BasicScene>/assets"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/examples/basic_scene/assets"
          "$<TARGET_FILE_DIR:BasicScene>/assets")

# Registry unit tests
add_executable(RegistryTests
	tests/RegistryTests.cpp
)
target_include_directories(RegistryTests PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
add_test(NAME RegistryTests COMMAND RegistryTests)

# SystemScheduler unit tests
add_executable(SystemTests
    tests/SystemTests.cpp
    src/ecs/system/SystemScheduler.cpp
)
target_include_directories(SystemTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
add_test(NAME SystemTests COMMAND SystemTests)


function(enable_strict_warnings target)
    if (MSVC)
        target_compile_options(${target} PRIVATE
            /W4        # high warnings
            /WX        # warnings as errors
            /permissive-
            /Zc:__cplusplus
            /EHsc
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
            -Wshadow
            -Wconversion
            -Wsign-conversion
            -Wcast-align
            -Woverloaded-virtual
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wnull-dereference
            -Wdouble-promotion
            -Wformat=2
            -Wimplicit-fallthrough
        )
    endif()
endfunction()

enable_strict_warnings(BasicScene)
enable_strict_warnings(RegistryTests)
enable_strict_warnings(SystemTests)