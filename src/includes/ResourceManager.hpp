#pragma once

#include <glad/glad.h>
#include <map>
#include <string>
#include "Shader.hpp"
#include "Texture.hpp"

class ResourceManager
{
public:
  static std::map<std::string, Shader> Shaders;
  static std::map<std::string, Texture> Textures;

  static Shader LoadShader(const char *vShaderFile, const char *fShaderFile, std::string name);
  static Shader GetShader(std::string name);

  static Texture LoadTexture(const char *file, bool alpha, std::string name);
  static Texture GetTexture(std::string name);

  static void Clear();

private:
  ResourceManager() {}
  static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile);
  static Texture loadTextureFromFile(const char *file, bool alpha);
};
