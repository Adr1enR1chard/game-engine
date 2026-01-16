#pragma once

#include <memory>

#include <model/MaterialInstance.hpp>
#include <model/Mesh.hpp>
#include <model/Model.hpp>

struct CMeshRenderer {
    std::shared_ptr<Mesh> mesh;
    MaterialInstance      material;
};

struct CModelRenderer {
    Model                         model;
    std::vector<MaterialInstance> materialOverrides;
};