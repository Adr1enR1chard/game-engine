#pragma once

#include <memory>

#include "handle/MeshHandle.hpp"
#include "handle/MaterialHandle.hpp"

struct CMeshRenderer
{
    std::shared_ptr<MeshHandle> mesh;
    std::shared_ptr<MaterialHandle> material;
};