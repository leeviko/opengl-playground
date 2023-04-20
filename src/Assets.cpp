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

  Sprite grass;
  grass.Atlas = Assets::Get().GetAtlas("atlas");
  grass.Offset = {0, 15};
  grass.Size = {16.0f, 16.0f};
  Assets::Get().SetSprite(GRASS, grass);

  Sprite guy;
  guy.Atlas = Assets::Get().GetAtlas("atlas");
  guy.Offset = {6, 15};
  guy.Size = {16.0f, 32.0f};
  Assets::Get().SetSprite(GUY, guy);
}

Sprite Assets::GetSprite(SpriteType name)
{
  return m_Sprites[name];
}

void Assets::SetSprite(SpriteType name, const Sprite &data)
{
  m_Sprites.emplace(name, data);
}
