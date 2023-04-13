#include "Texture.hpp"
#include "Renderer.hpp"

Texture::Texture() : width(0), height(0), internalFormat(GL_RGB), imageFormat(GL_RGB),
                     wrapS(GL_REPEAT), wrapT(GL_REPEAT), filterMin(GL_NEAREST), filterMax(GL_NEAREST)
{
  GLCall(glGenTextures(1, &textureId));
}

void Texture::Generate(uint32_t width, uint32_t height, unsigned char *data)
{
  this->width = width;
  this->height = height;

  GLCall(glBindTexture(GL_TEXTURE_2D, textureId));
  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax));

  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Bind() const
{
  GLCall(glBindTexture(GL_TEXTURE_2D, textureId));
}
