#pragma once

#include <string>
#include "MyMath.hpp"
#include <unordered_map>
#include "Texture.hpp"

enum SpriteType
{
  GRASS_01,
  GRASS_02,
  GRASS_03,
  PLAYER,
};

struct Sprite
{
  Texture Atlas;
  glm::vec2 Offset;
  glm::vec2 Size;
};

class Assets
{
public:
  Assets(const Assets &) = delete;
  static Assets &Get();

  void LoadAtlas(const char *path, std::string name);
  Texture GetAtlas(std::string name);

  Sprite GetSprite(SpriteType name);
  void SetSprite(SpriteType name, const Sprite &data);

  void LoadSprites();

private:
  Assets() {}
  std::unordered_map<SpriteType, Sprite> m_Sprites;
  std::unordered_map<std::string, Texture> m_Atlases;
};
