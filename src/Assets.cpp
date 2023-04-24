#include "Assets.hpp"
#include "ResourceManager.hpp"

Assets &Assets::Get()
{
  static Assets instance;
  return instance;
}

void Assets::LoadAtlas(const char *path, std::string name)
{
  Texture tex = ResourceManager::LoadTexture(path, true, name);

  m_Atlases.emplace(name, tex);
}

Texture Assets::GetAtlas(std::string name)
{
  return m_Atlases[name];
}

void Assets::LoadSprites()
{

  Sprite grass_01;
  grass_01.Atlas = Assets::Get().GetAtlas("atlas_01");
  grass_01.Offset = {0, 10};
  grass_01.Size = {16.0f, 16.0f};
  Assets::Get().SetSprite(GRASS_01, grass_01);

  Sprite grass_02;
  grass_02.Atlas = Assets::Get().GetAtlas("atlas_01");
  grass_02.Offset = {1, 10};
  grass_02.Size = {16.0f, 16.0f};
  Assets::Get().SetSprite(GRASS_02, grass_02);

  Sprite grass_03;
  grass_03.Atlas = Assets::Get().GetAtlas("atlas_01");
  grass_03.Offset = {2, 10};
  grass_03.Size = {16.0f, 16.0f};
  Assets::Get().SetSprite(GRASS_03, grass_03);

  Sprite guy;
  guy.Atlas = Assets::Get().GetAtlas("atlas_01");
  guy.Offset = {0, 0};
  guy.Size = {16.0f, 16.0f};
  Assets::Get().SetSprite(PLAYER, guy);
}

Sprite Assets::GetSprite(SpriteType name)
{
  return m_Sprites[name];
}

void Assets::SetSprite(SpriteType name, const Sprite &data)
{
  m_Sprites.emplace(name, data);
}
