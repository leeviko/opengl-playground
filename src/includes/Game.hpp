#pragma once
#include <iostream>
#include "Renderer.hpp"

class Game
{
private:
  Renderer renderer;
  Font font;
  uint32_t Width, Height;
  float m_ZoomLevel;

public:
  Game(uint32_t width, uint32_t height);
  ~Game();
  void Init();
  void Update(float dt);
  void ProcessInput(uint32_t f1KeyState, uint32_t f2KeyState);
};