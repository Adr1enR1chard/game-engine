/// @file main.cpp
/// @brief Entry point for the asset compiler tool. The tool processes and converts various asset files into optimized formats for use in the game engine.
/// @author Adrien RICHARD

#include <iostream>
#include <filesystem>
#include "utils/format.h"
#include "texture_compiler.h"

int main(int argc, char **argv)
{
    // Retrieve assets source and destination directories from command line arguments
    // Example: ./asset_compiler /path/to/source /path/to/destination
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <source_directory> <destination_directory>" << std::endl;
        return 1;
    }

    const char *sourceDir = argv[1];
    const char *destDir = argv[2];

    std::cout << "Asset Compiler Tool" << std::endl;
    std::cout << "Source Directory: " << sourceDir << std::endl;
    std::cout << "Destination Directory: " << destDir << std::endl;
    std::cout << "Starting asset processing..." << std::endl;

    // Iterate through the source directory, process each asset file, and save the optimized version to the destination directory
    for (const auto &entry : std::filesystem::recursive_directory_iterator(sourceDir))
    {
        if (entry.is_regular_file() && entry.path().has_extension())
        {
            const auto &filePath = entry.path();
            if (hasTextureExtension(filePath))
            {
                std::filesystem::path relativePath = std::filesystem::relative(filePath, sourceDir);
                std::filesystem::path outputPath = std::filesystem::path(destDir) / relativePath;
                outputPath.replace_extension(".asset");

                // Ensure the output directory exists
                std::filesystem::create_directories(outputPath.parent_path());

                // Compile the texture
                CompileTexture(filePath.string().c_str(), outputPath.string().c_str());
                std::cout << "Compiled texture: " << filePath << " -> " << outputPath << std::endl;
            }
            else if (hasModelExtension(filePath))
            {
                // Model compilation logic would go here
                // For now, just print the model file being processed
                std::cout << "Model processing not implemented yet: " << filePath << std::endl;
            }
            else
            {
                std::cout << "Unsupported file type, skipping: " << filePath << std::endl;
            }
        }
    }

    return 0;
}