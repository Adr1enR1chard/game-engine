#pragma once

#include <memory>

#include <engine/utils/RenderTypes.hpp>

struct CMeshRenderer
{
    MeshRef meshRef;
    MaterialRef materialRef;
};

struct CModelRenderer
{
    ModelRef modelRef;
    std::vector<MaterialRef> materialOverrides;
};