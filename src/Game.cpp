#include "Game.hpp"
#include "ResourceManager.hpp"
#include "GLFW/glfw3.h"
#include "Assets.hpp"
#include "Font.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

Game::Game(uint32_t width, uint32_t height) : Width(width), Height(height), m_ZoomLevel(5.0f), m_Pos({0.0f, 0.0f}), Keys(), ScrollOffset(0.0f)
{
}
Game::~Game()
{
  delete this->renderer;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Game::Init(GLFWwindow *window)
{
  ResourceManager::LoadShader("D:/Dev/Cpp/OpenGL/atlas/src/shaders/quad.vert", "D:/Dev/Cpp/OpenGL/atlas/src/shaders/quad.frag", "quad");
  ResourceManager::LoadShader("D:/Dev/Cpp/OpenGL/atlas/src/shaders/font.vert", "D:/Dev/Cpp/OpenGL/atlas/src/shaders/font.frag", "font");

  int samplers[32];
  for (unsigned int i = 0; i < 32; i++)
  {
    samplers[i] = i;
  }

  ResourceManager::GetShader("quad").Use().SetIntegerv("u_Textures", samplers);

  Assets::Get().LoadAtlas("D:/Dev/Cpp/OpenGL/atlas/src/assets/tilemap.png", "atlas_01");

  Assets::Get().LoadSprites();

  Font font;
  font.CreateAtlas("D:/Dev/Cpp/OpenGL/atlas/src/assets/fonts/Poppins.ttf");
  this->font = font;

  this->renderer = new Renderer();
  this->renderer->Init();

  this->m_Window = window;

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
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

  renderer->ResetStats();

  renderer->BeginBatch();

  for (int i = -25; i < 25; i++)
  {
    for (int j = -25; j < 25; j++)
    {
      renderer->DrawQuad({i, j}, {1.0f, 1.0f}, Assets::Get().GetSprite(GRASS_01));
    }
  }

  renderer->DrawText("Hello World!", &this->font, {0.0f, 0.0f}, 0.01f);
  renderer->DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, Assets::Get().GetSprite(GRASS_01));
  renderer->DrawQuad({1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.5f, 0.5f, 1.0f});
  renderer->DrawQuad({((left + right) / 2), ((top + bottom) / 2.0f)}, {1.0f, 1.0f}, Assets::Get().GetSprite(PLAYER));

  renderer->EndBatch();
  renderer->Flush();
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

void Game::ImGuiRender()
{
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  {
    ImGuiIO &io = ImGui::GetIO();

    ImGui::Begin("Statistics");

    ImGui::Text("App average: %.1f FPS", io.Framerate);
    ImGui::Text("Quads drawn: %d", this->renderer->GetStats().QuadCount);
    ImGui::Text("Draw calls:  %d", this->renderer->GetStats().DrawCalls);

    ImGui::End();
  }

  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(m_Window, &display_w, &display_h);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}