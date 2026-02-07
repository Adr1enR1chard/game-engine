#include <service/factory/ModelFactory.hpp>

#include <filesystem>

#include <engine/Engine.hpp>
#include <engine/utils/Log.hpp>
#include <assets_loader/model_loader.hpp>

namespace default_bundle
{
    using namespace engine;

    ModelHandle ModelFactory::LoadModel(const char *modelPath)
    {
        std::vector<MeshData> meshes;
        std::vector<MaterialDescriptor> materialsDesc;

        if (!ModelLoader::LoadModel(modelPath, meshes, materialsDesc))
        {
            Log::Print("Error while loading model: " + std::string(modelPath), LogLevel::Error);
            return {};
        }

        std::vector<MeshRef> meshRefs;
        std::vector<MaterialHandle> materials;
        std::filesystem::path modelDir =
            std::filesystem::current_path() / std::filesystem::path(modelPath).parent_path();
        for (size_t i = 0; i < meshes.size(); i++)
        {
            auto meshRef = m_meshFactory.Raw(meshes[i].vertices, meshes[i].indices, meshes[i].localTransform);
            auto materialParams = MaterialFactory::PBRMaterialParameters{};
            if (i < materialsDesc.size())
            {
                auto &matDesc = materialsDesc[i];
                if (strlen(matDesc.baseColorTexturePath) != 0)
                {
                    if (auto texture = m_textureFactory.Texture2D((modelDir / matDesc.baseColorTexturePath).string().c_str()))
                    {
                        materialParams.baseColorMap = texture;
                    }
                }
                if (matDesc.useMetallicRoughnessTexture)
                {
                    if (auto texture = m_textureFactory.Texture2D((modelDir / matDesc.metallicTexturePath).string().c_str()))
                    {
                        materialParams.metallicMap = texture;
                        materialParams.roughnessMap = texture;
                        materialParams.useMetallicRoughnessMap = true;
                    }
                }
                else
                {
                    if (auto texture = m_textureFactory.Texture2D((modelDir / matDesc.metallicTexturePath).string().c_str()))
                    {
                        materialParams.metallicMap = texture;
                    }
                    if (auto texture = m_textureFactory.Texture2D((modelDir / matDesc.roughnessTexturePath).string().c_str()))
                    {
                        materialParams.roughnessMap = texture;
                    }
                }
                if (strlen(matDesc.normalTexturePath) != 0)
                {
                    if (auto texture = m_textureFactory.Texture2D((modelDir / matDesc.normalTexturePath).string().c_str()))
                    {
                        materialParams.normalMap = texture;
                    }
                }
                if (strlen(matDesc.aoTexturePath) != 0)
                {
                    if (auto texture = m_textureFactory.Texture2D((modelDir / matDesc.aoTexturePath).string().c_str()))
                    {
                        materialParams.aoMap = texture;
                    }
                }
            }
            auto material = m_materialFactory.PBRMaterial(materialParams);
            meshRefs.push_back(meshRef);
            materials.push_back(material);
        }

        return {meshRefs, materials};
    }
} // namespace default_bundle
