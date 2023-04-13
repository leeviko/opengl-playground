#pragma once

#include <string>
#include "MyMath.hpp"
#include <unordered_map>

enum SpriteType
{
  VELHO,
  LEPAKKO
};

struct Sprite
{
  uint32_t AtlasTexId;
  glm::vec2 Offset;
  glm::vec2 Size;
};

class Assets
{
public:
  Assets(const Assets &) = delete;
  static Assets &Get();

  void LoadAtlas(const char *path, std::string name);
  uint32_t Assets::GetAtlas(std::string name);

  Sprite GetSprite(SpriteType name);
  void SetSprite(SpriteType name, Sprite data);

private:
  Assets() {}
  std::unordered_map<SpriteType, Sprite> m_SpriteData;
  std::unordered_map<std::string, uint32_t> m_Atlases;
};
