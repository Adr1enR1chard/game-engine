#pragma once

#include <memory>

#include "handle/MeshHandle.hpp"
#include "handle/MaterialHandle.hpp"

struct CMeshRenderer
{
    CMeshRenderer &setMesh(const std::shared_ptr<MeshHandle> &meshHandle)
    {
        mesh = meshHandle;
        return *this;
    }

    CMeshRenderer &setMaterial(const std::shared_ptr<MaterialHandle> &materialHandle)
    {
        material = materialHandle;
        return *this;
    }

    MeshHandle &getMesh() const
    {
        if (!mesh)
        {
            throw std::runtime_error("CMeshRenderer: Mesh is not set.");
        }
        return *mesh;
    }

    MaterialHandle &getMaterial() const
    {
        if (!material)
        {
            throw std::runtime_error("CMeshRenderer: Material is not set.");
        }
        return *material;
    }

private:
    std::shared_ptr<MeshHandle> mesh;
    std::shared_ptr<MaterialHandle> material;
};