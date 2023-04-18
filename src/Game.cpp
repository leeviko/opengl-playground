#include "Game.hpp"
#include "ResourceManager.hpp"
#include "GLFW/glfw3.h"
#include "Assets.hpp"
#include "Font.hpp"

Game::Game(uint32_t width, uint32_t height) : Width(width), Height(height), m_ZoomLevel(0.0f)
{
}
Game::~Game()
{
  ResourceManager::Clear();
  renderer.Clear();
}

void Game::Init()
{
  ResourceManager::LoadShader("D:/Dev/Cpp/OpenGL/atlas/src/shaders/quad.vert", "D:/Dev/Cpp/OpenGL/atlas/src/shaders/quad.frag", "quad");
  ResourceManager::LoadShader("D:/Dev/Cpp/OpenGL/atlas/src/shaders/font.vert", "D:/Dev/Cpp/OpenGL/atlas/src/shaders/font.frag", "font");

  int samplers[32];
  for (unsigned int i = 0; i < 32; i++)
  {
    samplers[i] = i;
  }

  ResourceManager::GetShader("quad").Use().SetIntegerv("u_Textures", samplers, false);

  Assets::Get().LoadAtlas("D:/Dev/Cpp/OpenGL/atlas/src/assets/tilemap.png", "atlas");

  Sprite velho;
  velho.AtlasTexId = Assets::Get().GetAtlas("atlas");
  velho.Offset = {0, 3};
  velho.Size = {16.0f, 16.0f};

  Assets::Get().SetSprite(VELHO, velho);

  Font font;
  font.CreateAtlas("D:/Dev/Cpp/OpenGL/atlas/src/assets/fonts/Poppins.ttf");
  this->font = font;

  Renderer renderer;
  this->renderer = renderer;
  this->renderer.Init();
}

void Game::Update(float dt)
{

  float aspectRatio = (float)Width / (float)Height;
  glm::mat4 proj = glm::ortho(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);

  ResourceManager::GetShader("quad").Use().SetMatrix4("u_MVP", proj, false);
  ResourceManager::GetShader("font").Use().SetMatrix4("proj", proj, false);

  renderer.BeginBatch();

  renderer.DrawText("Hello World!", &this->font, {0.0f, 0.0f}, 0.01f);

  renderer.DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, Assets::Get().GetSprite(VELHO));
  renderer.DrawQuad({1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.5f, 0.5f, 1.0f});

  renderer.EndBatch();
  renderer.Flush();
}

void Game::ProcessInput(uint32_t f1KeyState, uint32_t f2KeyState)
{
  if (f1KeyState == GLFW_PRESS)
  {
    m_ZoomLevel += 0.05f;
  }
  if (f2KeyState == GLFW_PRESS)
  {
    if (m_ZoomLevel >= 0.05f)
      m_ZoomLevel -= 0.05f;
  }
}