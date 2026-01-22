#pragma once

#include <engine/model/Service.hpp>
#include <utils/RenderTypes.hpp>

#include <string>
#include <unordered_map>

class ShaderResource : public Service
{
  public:
    ShaderResource()           = default;
    ~ShaderResource() override = default;

  public:
    ShaderRef create(const char* name, const char* vertexShaderPath, const char* fragmentShaderPath);

  private:
    std::unordered_map<ShaderRef, unsigned int> m_loadedShaders;
};