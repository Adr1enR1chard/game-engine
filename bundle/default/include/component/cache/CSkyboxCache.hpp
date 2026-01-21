#pragma once

#include <model/MaterialInstance.hpp>
#include <model/Mesh.hpp>
#include <model/Skybox.hpp>

struct CSkyboxCache {
    Skybox*               skyboxTexture;
    std::shared_ptr<Mesh> skyboxMesh;
    MaterialInstance      skyboxMaterial;
};