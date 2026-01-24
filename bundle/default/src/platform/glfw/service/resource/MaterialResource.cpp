#include <engine/utils/Log.hpp>
#include <glad/glad.h>
#include <service/resource/MaterialResource.hpp>

MaterialRef MaterialResource::create(ShaderRef shaderRef)
{
    MaterialRef newMaterialRef  = m_idManager.alloc();
    m_materials[newMaterialRef] = {shaderRef, {}};
    return newMaterialRef;
}

void MaterialResource::remove(MaterialRef materialRef)
{
    m_idManager.free(materialRef);
    m_materials.erase(materialRef);
}

void MaterialResource::setUniform(MaterialRef materialRef, const std::string& uniformName, const UniformValue& value)
{
    auto it = m_materials.find(materialRef);
    if (it == m_materials.end()) {
        Log::Print("Material not found", LogLevel::Warning, true);
        return;
    }

    std::visit(
        [&](auto const& v) {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, Uniform::DirectionalLight>) {
                setUniform(materialRef, uniformName + ".direction", v.direction);
                setUniform(materialRef, uniformName + ".color", v.color);
                setUniform(materialRef, uniformName + ".intensity", v.intensity);
            } else if constexpr (std::is_same_v<T, Uniform::PointLight>) {
                setUniform(materialRef, uniformName + ".position", v.position);
                setUniform(materialRef, uniformName + ".color", v.color);
                setUniform(materialRef, uniformName + ".intensity", v.intensity);
            } else {
                it->second.uniforms[uniformName] = value;
            }
        },
        value);
}

const UniformCollection* MaterialResource::getUniforms(MaterialRef materialRef) const
{
    auto it = m_materials.find(materialRef);
    if (it == m_materials.end()) {
        Log::Print("Material not found for materialRef " + std::to_string(materialRef) + " out of " +
                       std::to_string(m_materials.size()),
                   LogLevel::Warning);
        return nullptr;
    }
    return &(it->second.uniforms);
}

ShaderRef MaterialResource::getShaderRef(MaterialRef materialRef) const
{
    auto it = m_materials.find(materialRef);
    if (it == m_materials.end()) {
        Log::Print("Material not found", LogLevel::Warning, true);
        return 0;
    }
    return it->second.shaderRef;
}