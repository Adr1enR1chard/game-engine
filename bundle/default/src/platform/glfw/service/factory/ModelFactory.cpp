#include <service/factory/ModelFactory.hpp>

#include <filesystem>

#include <engine/Engine.hpp>
#include <engine/utils/Log.hpp>
#include <assets_loader/model_loader.hpp>

ModelRef ModelFactory::LoadModel(const char *modelPath)
{
    std::vector<std::vector<VertexLayout>> meshesVertices;
    std::vector<std::vector<unsigned int>> meshesIndices;
    std::vector<MaterialDescriptor> materials;

    if (!ModelLoader::LoadModel(modelPath, meshesVertices, meshesIndices, materials))
    {
        Log::Print("Error while loading model: " + std::string(modelPath), LogLevel::Error);
        return 0;
    }

    std::vector<MeshMaterialBinding> meshMaterialBindings;
    std::filesystem::path modelDir =
        std::filesystem::current_path() / std::filesystem::path(modelPath).parent_path();
    for (size_t i = 0; i < meshesVertices.size(); i++)
    {
        auto meshRef = m_meshFactory.Raw(meshesVertices[i], meshesIndices[i], glm::mat4(1.0f));
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
