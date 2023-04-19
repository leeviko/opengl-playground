#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.hpp"

class Game
{
private:
  Renderer renderer;
  Font font;
  uint32_t Width, Height;
  float m_ZoomLevel;
  glm::vec2 m_Pos;

public:
  bool Keys[348];
  float ScrollOffset;

  Game(uint32_t width, uint32_t height);
  ~Game();
  void Init();
  void Update(float dt);
  void HandleInput(float dt);
};