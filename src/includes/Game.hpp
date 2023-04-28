#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.hpp"

class Game
{
private:
  Renderer *renderer;
  Font font;
  float m_ZoomLevel;
  glm::vec2 m_Pos;
  GLFWwindow *m_Window;

public:
  uint32_t Width, Height;
  bool Keys[348];
  float ScrollOffset;
  glm::vec2 MousePos;

  Game(uint32_t width, uint32_t height);
  ~Game();
  void Init(GLFWwindow *window);
  void Update(float dt);
  void HandleInput(float dt);
  void ImGuiRender();
};
