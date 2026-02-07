#pragma once

#include <memory>

#include <engine/utils/RenderTypes.hpp>
#include <engine/bundle/core/service/factory/MaterialFactory.hpp>
#include <engine/bundle/core/service/factory/ModelFactory.hpp>

namespace engine
{

    struct CMesh
    {
        engine::MeshRef meshRef;
        MaterialHandle material;
    };

    struct CModel
    {
        ModelHandle model;
        std::vector<MaterialHandle> materialOverrides;
    };

} // namespace engine