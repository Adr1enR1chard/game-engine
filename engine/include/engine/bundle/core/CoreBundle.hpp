#pragma once

#include <engine/model/Bundle.hpp>

// --- Services ---
#include "service/Renderer.hpp"
#include "service/factory/MaterialFactory.hpp"
#include "service/factory/ModelFactory.hpp"
#include "service/factory/MeshFactory.hpp"
#include "service/factory/ShaderFactory.hpp"
#include "service/factory/TextureFactory.hpp"

// --- Systems ---
#include "system/CameraSystem.hpp"
#include "system/LightSystem.hpp"
#include "system/TransformSystem.hpp"

// --- Components ---
#include "component/CCamera.hpp"
#include "component/cache/CCameraCache.hpp"
#include "component/CTransform.hpp"
#include "component/cache/CTransformCache.hpp"
#include "component/CMesh.hpp"
#include "component/CEnvironment.hpp"
#include "component/CPointLight.hpp"
#include "component/CDirectionalLight.hpp"

namespace engine
{
    class CoreBundle : public Bundle
    {
    public:
        void install(Engine &engine) const override;
        void uninstall(Engine &engine) const override;

        std::vector<std::type_index> getRequiredBundles() const override
        {
            return {};
        }
    };
}