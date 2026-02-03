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
        Material material;
    };

    struct CModelRenderer
    {
        Model model;
        std::vector<Material> materialOverrides;
    };

} // namespace default_bundle