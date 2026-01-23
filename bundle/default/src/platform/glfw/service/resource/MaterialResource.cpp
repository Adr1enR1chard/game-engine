#include <engine/utils/Log.hpp>
#include <glad/glad.h>
#include <service/resource/MaterialResource.hpp>

MaterialRef MaterialResource::create(ShaderRef shaderRef)
{
    MaterialRef newMaterialRef  = m_idManager.alloc();
    m_materials[newMaterialRef] = shaderRef;
    return newMaterialRef;
}

void MaterialResource::remove(MaterialRef materialRef)
{
    m_idManager.free(materialRef);
    m_materials.erase(materialRef);
    m_materialUniforms.erase(materialRef);
}

void MaterialResource::setUniform(MaterialRef materialRef, const char* uniformName, const UniformValue& value)
{
    auto it = m_materials.find(materialRef);
    if (it == m_materials.end()) {
        Log::Print("Material not found", LogLevel::Warning, true);
        return;
    }

    m_materialUniforms[materialRef][uniformName] = value;
}

const UniformCollection* MaterialResource::getUniforms(MaterialRef materialRef) const
{
    auto it = m_materialUniforms.find(materialRef);
    if (it == m_materialUniforms.end()) {
        Log::Print("Material uniforms not found", LogLevel::Warning, true);
        return nullptr;
    }
    return &(it->second);
}

ShaderRef MaterialResource::getShaderRef(MaterialRef materialRef) const
{
    auto it = m_materials.find(materialRef);
    if (it == m_materials.end()) {
        Log::Print("Material not found", LogLevel::Warning, true);
        return 0;
    }
    return it->second;
}