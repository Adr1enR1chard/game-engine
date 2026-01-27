
#pragma once
#include <filesystem>

const char *TEXTURE_EXTENSIONS[] = {".png", ".jpg", ".jpeg", ".tga", ".bmp", ".gif"};
const char *MODEL_EXTENSIONS[] = {".obj", ".fbx", ".gltf", ".glb", ".dae", ".3ds"};

bool hasTextureExtension(const std::filesystem::path &path)
{
    for (const auto &ext : TEXTURE_EXTENSIONS)
    {
        if (path.extension() == ext)
            return true;
    }
    return false;
}

bool hasModelExtension(const std::filesystem::path &path)
{
    for (const auto &ext : MODEL_EXTENSIONS)
    {
        if (path.extension() == ext)
            return true;
    }
    return false;
}
