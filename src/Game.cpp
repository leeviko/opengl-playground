#include "Game.hpp"
#include "ResourceManager.hpp"
#include "GLFW/glfw3.h"
#include "Assets.hpp"
#include "Font.hpp"

Game::Game(uint32_t width, uint32_t height) : Width(width), Height(height), m_ZoomLevel(5.0f), m_Pos({0.0f, 0.0f}), Keys(), ScrollOffset(0.0f)
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

  ResourceManager::GetShader("quad").Use().SetIntegerv("u_Textures", samplers);

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
  float left = -aspectRatio * m_ZoomLevel + m_Pos.x;
  float right = aspectRatio * m_ZoomLevel + m_Pos.x;
  float top = -m_ZoomLevel + m_Pos.y;
  float bottom = m_ZoomLevel + m_Pos.y;

  glm::mat4 proj = glm::ortho(left, right, top, bottom, -1.0f, 1.0f);

  ResourceManager::GetShader("quad").Use().SetMatrix4("u_MVP", proj);
  ResourceManager::GetShader("font").Use().SetMatrix4("proj", proj);

  renderer.BeginBatch();

  renderer.DrawText("Hello World!", &this->font, {0.0f, 0.0f}, 0.01f);

  renderer.DrawQuad({((left + right) / 2.0f), ((top + bottom) / 2.0f)}, {1.0f, 1.0f}, Assets::Get().GetSprite(VELHO));
  renderer.DrawQuad({1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.5f, 0.5f, 1.0f});

  renderer.EndBatch();
  renderer.Flush();
}

void Game::HandleInput(float dt)
{
  if (ScrollOffset > 0.0f && m_ZoomLevel > 1.0f)
    m_ZoomLevel -= ScrollOffset * 15.0f * dt;
  else if (ScrollOffset < 0.0f && m_ZoomLevel < 10.0f)
    m_ZoomLevel -= ScrollOffset * 15.0f * dt;
  ScrollOffset = 0.0f;

  if (Keys[GLFW_KEY_A])
  {
    m_Pos.x -= 10.0f * dt;
  }
  else if (Keys[GLFW_KEY_D])
  {
    m_Pos.x += 10.0f * dt;
  }

  if (Keys[GLFW_KEY_W])
  {
    m_Pos.y += 10.0f * dt;
  }
  else if (Keys[GLFW_KEY_S])
  {
    m_Pos.y -= 10.0f * dt;
  }
}