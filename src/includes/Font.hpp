#pragma once
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "MyMath.hpp"
#include <unordered_map>

struct Character
{
  glm::vec2 Size;
  glm::vec2 Bitmap;
  glm::vec2 Advance;
  float TexOffset;
};

struct TextVertex
{
  glm::vec3 Position;
  glm::vec2 TexCoords;
};

class Font
{
private:
  FT_Library m_Ft;

public:
  uint32_t atlasTexId;
  uint32_t atlasWidth;
  uint32_t atlasHeight;
  std::unordered_map<unsigned char, Character> Characters;

  Font();
  ~Font();

  void CreateAtlas(std::string font);
};
