#include <engine/service/resource/ModelResource.hpp>

#include <engine/utils/Log.hpp>

ModelRef ModelResource::create(std::vector<MeshMaterialBinding> meshMaterialBindings)
{
    ModelRef newModelRef = m_idManager.alloc();
    m_modelMeshes[newModelRef] = std::move(meshMaterialBindings);
    return newModelRef;
}

void ModelResource::forEach(ModelRef modelRef, const std::function<void(MeshRef, MaterialRef, size_t)> &func) const
{
    auto it = m_modelMeshes.find(modelRef);
    if (it == m_modelMeshes.end())
    {
        return;
    }

    for (size_t i = 0; i < it->second.size(); ++i)
    {
        const auto &meshMaterialBinding = it->second[i];
        func(meshMaterialBinding.meshRef, meshMaterialBinding.materialRef, i);
    }
}