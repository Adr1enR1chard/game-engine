#pragma once

#include <cstdint>
#include <string>
#include <vector>

class AssetsLoader
{
    struct TextureHeader
    {
        uint32_t magic;
        uint32_t width;
        uint32_t height;
        uint32_t format;
        uint32_t mipLevels;
    };

public:
    static bool LoadTextureAsset(const std::string &filePath, unsigned int &width, unsigned int &height,
                                 unsigned int &channels, std::vector<unsigned char> &data);
    static std::string getAssetPath(const std::string &filePath);
};