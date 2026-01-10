#pragma once

#include <memory>

#include <engine/Material.hpp>
#include <engine/Mesh.hpp>
#include <engine/Model.hpp>

struct CMeshRenderer {
    std::shared_ptr<Mesh>     mesh;
    std::shared_ptr<Material> material;
    Model                     model;
};