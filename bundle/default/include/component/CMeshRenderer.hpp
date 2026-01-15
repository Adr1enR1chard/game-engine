#pragma once

#include <memory>

#include <engine/model/MaterialInstance.hpp>
#include <engine/model/Mesh.hpp>
#include <engine/model/Model.hpp>

struct CMeshRenderer {
    std::shared_ptr<Mesh> mesh;
    MaterialInstance      material;
};

struct CModelRenderer {
    Model                         model;
    std::vector<MaterialInstance> materialOverrides;
};