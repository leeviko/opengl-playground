#include "Renderer.hpp"
#include <glad/glad.h>
#include <array>

void GLClearError()
{
  while (glGetError() != GL_NO_ERROR)
    ;
}
bool GLLogCall(const char *function, const char *file, int line)
{
  while (GLenum error = glGetError())
  {
    std::cout << "[OpenGL Error] (" << error << ")"
              << " " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}

struct RendererData
{
  unsigned int QuadVA = 0;
  unsigned int QuadVB = 0;
  unsigned int QuadIB = 0;

  unsigned int WhiteTextureId = 0;
  unsigned int WhiteTextureSlot = 0;

  unsigned int TextureIndex = 1;

  Vertex *QuadBuffer = nullptr;
  Vertex *QuadBufferPtr = nullptr;

  unsigned int IndexCount = 0;

  glm::vec4 QuadVertexPositions[4];

  std::array<unsigned int, MaxTextureSlots> TextureSlots;
};

RendererData s_Data;

Renderer::Renderer()
{
}
Renderer::~Renderer()
{
}

void Renderer::Init()
{
  s_Data.QuadBuffer = new Vertex[MaxVertices];

  GLCall(glGenVertexArrays(1, &s_Data.QuadVA));
  GLCall(glGenBuffers(1, &s_Data.QuadVB));

  GLCall(glBindVertexArray(s_Data.QuadVA));

  GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
  GLCall(glBufferData(GL_ARRAY_BUFFER, MaxVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

  GLCall(glEnableVertexAttribArray(0));
  GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position)));
  GLCall(glEnableVertexAttribArray(1));
  GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Color)));
  GLCall(glEnableVertexAttribArray(2));
  GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords)));
  GLCall(glEnableVertexAttribArray(3));
  GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexIndex)));

  unsigned int indices[MaxIndices];

  unsigned int offset = 0;
  for (int i = 0; i < MaxIndices; i += 6)
  {
    indices[i + 0] = 0 + offset;
    indices[i + 1] = 1 + offset;
    indices[i + 2] = 2 + offset;
    indices[i + 3] = 2 + offset;
    indices[i + 4] = 3 + offset;
    indices[i + 5] = 0 + offset;

    offset += 4;
  }

  GLCall(glGenBuffers(1, &s_Data.QuadIB));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadIB));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

  GLCall(glGenTextures(1, &s_Data.WhiteTextureId));
  GLCall(glBindTexture(GL_TEXTURE_2D, s_Data.WhiteTextureId));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  uint32_t color = 0xffffffff;
  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color));

  s_Data.TextureSlots[0] = s_Data.WhiteTextureId;
  for (int i = 1; i < MaxTextureSlots; i++)
  {
    s_Data.TextureSlots[i] = 0;
  }

  s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
  s_Data.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
  s_Data.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
  s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};

  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Renderer::BeginBatch()
{
  s_Data.QuadBufferPtr = s_Data.QuadBuffer;
}
void Renderer::EndBatch()
{
  GLsizeiptr size = (uint8_t *)s_Data.QuadBufferPtr - (uint8_t *)s_Data.QuadBuffer;
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.QuadBuffer));
}
void Renderer::Flush()
{
  for (unsigned int i = 0; i < s_Data.TextureIndex; i++)
  {
    GLCall(glActiveTexture(GL_TEXTURE0 + i));
    GLCall(glBindTexture(GL_TEXTURE_2D, s_Data.TextureSlots[i]));
  }

  GLCall(glBindVertexArray(s_Data.QuadVA));
  GLCall(glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, 0));

  s_Data.IndexCount = 0;
  s_Data.TextureIndex = 1;

  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
void Renderer::DrawQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color)
{
  if (s_Data.IndexCount >= MaxIndices)
  {
    EndBatch();
    Flush();
    BeginBatch();
  }

  glm::mat4 transform = glm::translate(glm::mat4(1.0f), {pos.x, pos.y, 0.0f}) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

  float textureIndex = 0.0f;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {0.0f, 0.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {1.0f, 0.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {1.0f, 1.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {0.0f, 1.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.IndexCount += 6;
}

void Renderer::DrawQuad(const glm::vec2 &pos, const glm::vec2 &size, uint32_t textureId)
{
  if (s_Data.IndexCount >= MaxIndices || s_Data.TextureIndex > 31)
  {
    EndBatch();
    Flush();
    BeginBatch();
  }
  constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

  glm::mat4 transform = glm::translate(glm::mat4(1.0f), {pos.x, pos.y, 0.0f}) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

  float textureIndex = 0.0f;
  for (unsigned int i = 1; i < s_Data.TextureIndex; i++)
  {
    if (s_Data.TextureSlots[i] == textureId)
    {
      textureIndex = (float)i;
      break;
    }
  }

  float x = 0, y = 3;
  float sheetWidth = 192.0f, sheetHeight = 176.0f;
  float spriteWidth = 16.0f, spriteHeight = 16.0f;

  if (textureIndex == 0.0f)
  {
    textureIndex = (float)s_Data.TextureIndex;
    s_Data.TextureSlots[s_Data.TextureIndex] = textureId;
    s_Data.TextureIndex++;
  }

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {0.0f, 0.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {1.0f, 0.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {1.0f, 1.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {0.0f, 1.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.IndexCount += 6;
}

void Renderer::DrawQuad(const glm::vec2 &pos, const glm::vec2 &size, Sprite sprite)
{
  if (s_Data.IndexCount >= MaxIndices || s_Data.TextureIndex > 31)
  {
    EndBatch();
    Flush();
    BeginBatch();
  }
  constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

  glm::mat4 transform = glm::translate(glm::mat4(1.0f), {pos.x, pos.y, 0.0f}) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

  float textureIndex = 0.0f;
  for (unsigned int i = 1; i < s_Data.TextureIndex; i++)
  {
    if (s_Data.TextureSlots[i] == sprite.AtlasTexId)
    {
      textureIndex = (float)i;
      break;
    }
  }

  if (textureIndex == 0.0f)
  {
    textureIndex = (float)s_Data.TextureIndex;
    s_Data.TextureSlots[s_Data.TextureIndex] = sprite.AtlasTexId;
    s_Data.TextureIndex++;
  }

  std::cout << "LOL: " << ((sprite.Offset.x + 1) * sprite.Size.x) / 192.0f << std::endl;
  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {(sprite.Offset.x * sprite.Size.x) / 192.0f, (sprite.Offset.y * sprite.Size.y) / 176.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {((sprite.Offset.x + 1) * sprite.Size.x) / 192.0f, (sprite.Offset.y * sprite.Size.y) / 176.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {((sprite.Offset.x + 1) * sprite.Size.x) / 192.0f, ((sprite.Offset.y + 1) * sprite.Size.y) / 176.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {(sprite.Offset.x * sprite.Size.x) / 192.0f, ((sprite.Offset.y + 1) * sprite.Size.y) / 176.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.IndexCount += 6;
}