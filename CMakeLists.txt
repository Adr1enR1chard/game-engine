cmake_minimum_required(VERSION 3.20)

# Project
project(OpenGLTutorial LANGUAGES C CXX)

# Standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Output directories (useful for Visual Studio multi-config)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Sources
add_executable(OpenGLTutorial
	src/main.cpp
	src/include/glad.c
	src/include/stb_image.cpp
)

# Include directories for local headers (GLAD, etc.)
target_include_directories(OpenGLTutorial PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Avoid GLFW including GL headers; we provide them via GLAD
target_compile_definitions(OpenGLTutorial PRIVATE GLFW_INCLUDE_NONE)

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

# Link libraries
target_link_libraries(OpenGLTutorial PRIVATE
		glfw
		OpenGL::GL
)

# Windows-specific (ensure opengl32 linkage where necessary)
if (WIN32)
	target_link_libraries(OpenGLTutorial PRIVATE opengl32)
endif()

add_custom_command(TARGET OpenGLTutorial POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:OpenGLTutorial>/assets"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/assets"
          "$<TARGET_FILE_DIR:OpenGLTutorial>/assets")
