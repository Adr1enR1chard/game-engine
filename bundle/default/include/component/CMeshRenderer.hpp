#pragma once

#include <memory>

#include <model/MaterialInstance.hpp>
#include <model/Mesh.hpp>
#include <model/Model.hpp>

struct CMeshRenderer {
    MeshRef          meshRef;
    MaterialInstance material;
};

struct CModelRenderer {
    Model                         model;
    std::vector<MaterialInstance> materialOverrides;
};