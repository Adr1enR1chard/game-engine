#pragma once

#include <engine/model/Bundle.hpp>

// --- Services ---
#include "service/Renderer.hpp"

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