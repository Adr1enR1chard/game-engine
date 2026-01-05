#pragma once

#include <memory>

#include "ecs/component/CComponent.hpp"
#include "handle/MeshHandle.hpp"
#include "handle/MaterialHandle.hpp"

struct CMeshRenderer : CComponent
{
    std::shared_ptr<MeshHandle> mesh;
    std::shared_ptr<MaterialHandle> material;
};