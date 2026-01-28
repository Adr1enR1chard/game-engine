cmake_minimum_required(VERSION 3.20)

project(AssetCompiler LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(AssetCompiler
    src/main.cpp
    src/texture_compiler.cpp
    src/mesh_compiler.cpp
    src/model_compiler.cpp
    src/third_party/stb_image/stb_image.cpp
)

target_include_directories(AssetCompiler PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/include
)
# Call the texture compiler after building the AssetCompiler executable
add_custom_command(
    TARGET AssetCompiler POST_BUILD
    COMMAND AssetCompiler ${CMAKE_SOURCE_DIR}/test ${CMAKE_BINARY_DIR}/compiled_assets
    COMMENT "Running AssetCompiler to process assets..."
)

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

target_link_libraries(AssetCompiler PUBLIC
    assimp::assimp
)