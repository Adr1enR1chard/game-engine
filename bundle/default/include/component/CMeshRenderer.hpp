#pragma once

#include <memory>

#include <engine/utils/RenderTypes.hpp>
#include <service/factory/MaterialFactory.hpp>
#include <service/factory/ModelFactory.hpp>

namespace default_bundle
{

    struct CMeshRenderer
    {
        engine::MeshRef meshRef;
        MaterialHandle material;
    };

    struct CModelRenderer
    {
        Model model;
        std::vector<MaterialHandle> materialOverrides;
    };

} // namespace default_bundle