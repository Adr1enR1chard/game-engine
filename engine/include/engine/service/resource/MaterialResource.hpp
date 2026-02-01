#pragma once

#include <engine/model/Service.hpp>
#include <engine/service/resource/ShaderResource.hpp>
#include <string>
#include <unordered_map>
#include <engine/utils/IdManager.hpp>
#include <engine/utils/RenderTypes.hpp>

namespace engine
{

  class MaterialResource : public Service
  {
  public:
    MaterialResource() = default;
    ~MaterialResource() override = default;

    MaterialRef create(ShaderRef shaderRef);
    void remove(MaterialRef materialRef);

    void setUniform(MaterialRef materialRef, const std::string &uniformName, const UniformValue &value);
    const UniformCollection *getUniforms(MaterialRef materialRef) const;
    ShaderRef getShaderRef(MaterialRef materialRef) const;

  private:
    struct MaterialData
    {
      ShaderRef shaderRef;
      UniformCollection uniforms;
    };

    IdManager m_idManager;
    std::unordered_map<MaterialRef, MaterialData> m_materials;
  };

} // namespace engine