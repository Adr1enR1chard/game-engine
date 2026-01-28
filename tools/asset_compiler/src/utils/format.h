
#pragma once
#include <filesystem>

const char *TEXTURE_EXTENSIONS[] = {".png", ".jpg", ".jpeg", ".tga", ".bmp", ".gif"};
const char *MODEL_EXTENSIONS[] = {".obj", ".fbx", ".gltf", ".glb", ".dae", ".3ds"};

enum class AssetType
{
    Texture,
    Model,
    Unknown
};

AssetType GetAssetType(const std::filesystem::path &path)
{
    for (const auto &ext : TEXTURE_EXTENSIONS)
    {
        if (path.extension() == ext)
            return AssetType::Texture;
    }
    for (const auto &ext : MODEL_EXTENSIONS)
    {
        if (path.extension() == ext)
            return AssetType::Model;
    }
    return AssetType::Unknown;
}
