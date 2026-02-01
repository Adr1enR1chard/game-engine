#pragma once

#include <memory>

#include <engine/utils/RenderTypes.hpp>

namespace default_bundle
{

    struct CMeshRenderer
    {
        engine::MeshRef meshRef;
        engine::MaterialRef materialRef;
    };

    struct CModelRenderer
    {
        engine::ModelRef modelRef;
        std::vector<engine::MaterialRef> materialOverrides;
    };

} // namespace default_bundle