#include "Assets.hpp"
#include "ResourceManager.hpp"

Assets &Assets::Get()
{
  static Assets instance;
  return instance;
}

void Assets::LoadAtlas(const char *path, std::string name)
{
  uint32_t texId = ResourceManager::LoadTexture(path, true, name).textureId;

  m_Atlases.insert({name, texId});
}

uint32_t Assets::GetAtlas(std::string name)
{
  return m_Atlases[name];
}

Sprite Assets::GetSprite(SpriteType name)
{
  return m_SpriteData[name];
}

void Assets::SetSprite(SpriteType name, Sprite data)
{
  m_SpriteData.insert({name, data});
}
