#pragma once
#include <cstdint>

struct TextureHeader
{
    uint32_t magic; // 'TEX0'
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    uint32_t mipLevels; // 1 for now
};

void CompileTexture(const char *inputPath, const char *outputPath);