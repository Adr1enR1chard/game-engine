#pragma once

#include <memory>

#include "ecs/component/CComponent.hpp"
#include "handle/MeshHandle.hpp"
#include "handle/ShaderHandle.hpp"

struct CMeshRenderer : CComponent
{
    std::shared_ptr<MeshHandle> mesh;
    std::shared_ptr<ShaderHandle> shader;
};