#pragma once
#include <iostream>
#include "MyMath.hpp"
#include "Macros.hpp"
#include "Assets.hpp"

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

static const uint32_t MaxQuads = 1000;
static const uint32_t MaxVertices = MaxQuads * 4;
static const uint32_t MaxIndices = MaxQuads * 6;
static const uint32_t MaxTextureSlots = 32;

struct TileData
{
  uint32_t X = 0, Y = 0;
  float SheetWidth = 192.0f, SheetHeight = 176.0f;
  float SpriteWidth = 16.0f, SpriteHeight = 16.0f;
};

struct Vertex
{
  glm::vec3 Position;
  glm::vec4 Color;
  glm::vec2 TexCoords;
  float TexIndex;
};

class Renderer
{
public:
  Renderer();
  ~Renderer();

  void Init();
  void BeginBatch();
  void Flush();
  void EndBatch();

  void DrawQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color);
  void DrawQuad(const glm::vec2 &pos, const glm::vec2 &size, uint32_t textureId);
  void DrawQuad(const glm::vec2 &pos, const glm::vec2 &size, Sprite sprite);
};