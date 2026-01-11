#pragma once

#include <memory>

#include <engine/MaterialInstance.hpp>
#include <engine/Mesh.hpp>
#include <engine/Model.hpp>

struct CMeshRenderer {
    std::shared_ptr<Mesh> mesh;
    MaterialInstance      material;
};

struct CModelRenderer {
    Model                         model;
    std::vector<MaterialInstance> materialOverrides;
};