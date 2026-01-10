#pragma once

#include <memory>

#include <engine/Mesh.hpp>
#include <engine/Material.hpp>
#include <engine/Model.hpp>

struct CMeshRenderer
{
    CMeshRenderer &setMesh(const std::shared_ptr<Mesh> &meshHandle)
    {
        mesh = meshHandle;
        return *this;
    }

    CMeshRenderer &setMaterial(const std::shared_ptr<Material> &materialHandle)
    {
        material = materialHandle;
        return *this;
    }

    Mesh &getMesh() const
    {
        if (!mesh)
        {
            throw std::runtime_error("CMeshRenderer: Mesh is not set.");
        }
        return *mesh;
    }

    Material &getMaterial() const
    {
        if (!material)
        {
            throw std::runtime_error("CMeshRenderer: Material is not set.");
        }
        return *material;
    }

    Model &getModel()
    {
        return model;
    }

    Model &setModel(const Model &modelHandle)
    {
        model = modelHandle;
        return model;
    }

private:
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    Model model;
};