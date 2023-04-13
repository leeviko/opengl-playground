#include "Game.hpp"
#include "ResourceManager.hpp"
#include "GLFW/glfw3.h"
#include "Assets.hpp"

Game::Game(uint32_t width, uint32_t height) : Width(width), Height(height), m_ZoomLevel(0.0f)
{
}
Game::~Game()
{
  ResourceManager::Clear();
}

void Game::Init()
{
  ResourceManager::LoadShader("D:/Dev/Cpp/OpenGL/atlas/src/shaders/shader.vert", "D:/Dev/Cpp/OpenGL/atlas/src/shaders/shader.frag", "shader");

  int samplers[32];
  for (unsigned int i = 0; i < 32; i++)
  {
    samplers[i] = i;
  }

  ResourceManager::GetShader("shader").Use();
  ResourceManager::GetShader("shader").SetIntegerv("u_Textures", samplers, false);

  Assets::Get().LoadAtlas("D:/Dev/Cpp/OpenGL/atlas/src/assets/tilemap.png", "atlas");

  Sprite velho;
  velho.AtlasTexId = Assets::Get().GetAtlas("atlas");
  velho.Offset = {0, 3};
  velho.Size = {16.0f, 16.0f};

  Assets::Get().SetSprite(VELHO, velho);

  Renderer renderer;
  this->renderer = renderer;
  this->renderer.Init();
}

void Game::Update(float dt)
{
  ResourceManager::GetShader("shader").Use();

  float aspectRatio = (float)Width / (float)Height;
  glm::mat4 proj = glm::ortho(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);

  ResourceManager::GetShader("shader").SetMatrix4("u_MVP", proj, false);

  renderer.BeginBatch();

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