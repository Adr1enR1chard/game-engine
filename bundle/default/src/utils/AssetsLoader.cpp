#include <utils/AssetsLoader.hpp>
#include <engine/utils/Log.hpp>

bool AssetsLoader::LoadTextureAsset(const std::string &filePath, unsigned int &width, unsigned int &height,
                                    unsigned int &channels, std::vector<unsigned char> &data)
{
    FILE *file;
    std::string assetPath = getAssetPath(filePath);
    errno_t err = fopen_s(&file, assetPath.c_str(), "rb");
    if (err != 0)
    {
        return false;
    }

    TextureHeader header;
    fread(&header, sizeof(header), 1, file);
    width = header.width;
    height = header.height;
    channels = header.format;

    data.resize(width * height * channels);
    fread(data.data(), sizeof(unsigned char), width * height * channels, file);
    fclose(file);

    return true;
}

std::string AssetsLoader::getAssetPath(const std::string &filePath)
{
    return filePath + ".asset";
}