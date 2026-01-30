#include <service/factory/ModelFactory.hpp>

#include <filesystem>

#include <engine/Engine.hpp>
#include <engine/utils/Log.hpp>
#include <assets_loader/model_loader.hpp>

ModelRef ModelFactory::LoadModel(const char *modelPath)
{
    std::vector<MeshData> meshes;
    std::vector<MaterialDescriptor> materials;

    if (!ModelLoader::LoadModel(modelPath, meshes, materials))
    {
        Log::Print("Error while loading model: " + std::string(modelPath), LogLevel::Error);
        return 0;
    }

    std::vector<MeshMaterialBinding> meshMaterialBindings;
    std::filesystem::path modelDir =
        std::filesystem::current_path() / std::filesystem::path(modelPath).parent_path();
    for (size_t i = 0; i < meshes.size(); i++)
    {
        auto meshRef = m_meshFactory.Raw(meshes[i].vertices, meshes[i].indices, meshes[i].localTransform);
        auto materialOptions = PBRMaterialParameters{};
        if (i < materials.size())
        {
            auto &matDesc = materials[i];
            if (strlen(matDesc.baseColorTexturePath) != 0)
            {
                if (auto texture = m_textureFactory.Texture2D((modelDir / matDesc.baseColorTexturePath).string().c_str()))
                {
                    materialOptions.baseColorMap = texture;
                }
            }
            if (matDesc.useMetallicRoughnessTexture)
            {
                if (auto texture = m_textureFactory.Texture2D((modelDir / matDesc.metallicTexturePath).string().c_str()))
                {
                    materialOptions.metallicMap = texture;
                    materialOptions.roughnessMap = texture;
                    materialOptions.useMetallicRoughnessMap = true;
                }
            }
            else
            {
                if (auto texture = m_textureFactory.Texture2D((modelDir / matDesc.metallicTexturePath).string().c_str()))
                {
                    materialOptions.metallicMap = texture;
                }
                if (auto texture = m_textureFactory.Texture2D((modelDir / matDesc.roughnessTexturePath).string().c_str()))
                {
                    materialOptions.roughnessMap = texture;
                }
            }
            if (strlen(matDesc.normalTexturePath) != 0)
            {
                if (auto texture = m_textureFactory.Texture2D((modelDir / matDesc.normalTexturePath).string().c_str()))
                {
                    materialOptions.normalMap = texture;
                }
            }
            if (strlen(matDesc.aoTexturePath) != 0)
            {
                if (auto texture = m_textureFactory.Texture2D((modelDir / matDesc.aoTexturePath).string().c_str()))
                {
                    materialOptions.aoMap = texture;
                }
            }
        }
        auto materialRef = m_materialFactory.PBRMaterial(materialOptions);
        meshMaterialBindings.push_back({meshRef, materialRef});
    }

    return m_modelResource.create(meshMaterialBindings);
}
