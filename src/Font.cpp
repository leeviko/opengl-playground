#include "Font.hpp"
#include "Renderer.hpp"
#include <glad/glad.h>
#include <iostream>

Font::Font()
{
}
Font::~Font()
{
}

void Font::CreateAtlas(std::string font)
{
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  {
    std::cout << "ERROR::FREETYPE: Could not init FreeType library" << std::endl;
    return;
  }
  m_Ft = ft;

  FT_Face face;
  if (FT_New_Face(ft, font.c_str(), 0, &face))
  {
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    return;
  }

  FT_Set_Pixel_Sizes(face, 0, 48);

  uint32_t width = 0;
  uint32_t height = 0;

  for (uint32_t i = 32; i < 128; i++)
  {
    if (FT_Load_Char(face, i, FT_LOAD_RENDER))
    {
      std::cout << "Loading character " << i << " failed" << std::endl;
      continue;
    }

    width += face->glyph->bitmap.width;
    height = std::max(height, face->glyph->bitmap.rows);
  }

  atlasWidth = width;
  atlasHeight = height;

  uint32_t texId;
  GLCall(glGenTextures(1, &texId));
  GLCall(glBindTexture(GL_TEXTURE_2D, texId));
  GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

  atlasTexId = texId;

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

  uint32_t x = 0;

  for (unsigned char c = 32; c < 128; c++)
  {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      continue;
    }
    FT_GlyphSlot g = face->glyph;

    GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer));

    Characters.emplace(c, Character{
                              {g->bitmap.width, g->bitmap.rows},
                              {g->bitmap_left, g->bitmap_top},
                              {g->advance.x >> 6, g->advance.y >> 6},
                              (float)x / width});

    x += face->glyph->bitmap.width;
  }
}