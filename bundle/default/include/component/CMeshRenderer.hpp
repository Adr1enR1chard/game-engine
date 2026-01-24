#pragma once

#include <memory>

#include <utils/RenderTypes.hpp>

struct CMeshRenderer {
    MeshRef     meshRef;
    MaterialRef materialRef;
};

struct CModelRenderer {
    ModelRef                 modelRef;
    std::vector<MaterialRef> materialOverrides;
};