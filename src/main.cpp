#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Game.hpp"

uint32_t SCREEN_WIDTH = 1270;
uint32_t SCREEN_HEIGHT = 720;

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Testing", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  int samplers[32];
  for (int i = 0; i < 32; i++)
  {
    samplers[i] = i;
  }

  Game game(SCREEN_WIDTH, SCREEN_HEIGHT);

  game.Init();

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);

    float currFrame = glfwGetTime();
    deltaTime = currFrame - lastFrame;
    lastFrame = currFrame;

    uint32_t f1KeyState = glfwGetKey(window, GLFW_KEY_F1);
    uint32_t f2KeyState = glfwGetKey(window, GLFW_KEY_F2);

    game.ProcessInput(f1KeyState, f2KeyState);

    game.Update(deltaTime);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}