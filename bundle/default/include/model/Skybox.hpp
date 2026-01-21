#pragma once
#include <model/Texture.hpp>

#include <string>
#include <vector>

class Skybox : public Texture
{
  public:
    Skybox(std::vector<std::string> faces);
};