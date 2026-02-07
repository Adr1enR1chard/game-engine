#pragma once

#include <engine/model/Bundle.hpp>

// --- Systems ---
#include <system/EnvironmentSystem.hpp>
#include <system/RenderSystem.hpp>

// --- Services ---
#include <service/ShadowMapping.hpp>

#include <engine/bundle/core/CoreBundle.hpp>

namespace rasterization_bundle
{

  using namespace engine;

  class RasterizationBundle : public Bundle
  {
  public:
    void install(Engine &engine) const override;
    void uninstall(Engine &engine) const override;

    std::vector<std::type_index> getRequiredBundles() const override
    {
      return {std::type_index(typeid(CoreBundle))};
    }
  };

} // namespace rasterization_bundle