#pragma once
#include <iostream>
#include <glad/glad.h>

class Texture
{
public:
  uint32_t textureId;
  uint32_t width, height;

  uint32_t internalFormat;
  uint32_t imageFormat;

  uint32_t wrapS, wrapT;
  uint32_t filterMin, filterMax;

  Texture();

  void Generate(uint32_t width, uint32_t height, unsigned char *data);
  void Bind() const;
};
