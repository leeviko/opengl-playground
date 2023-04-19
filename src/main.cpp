#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Game.hpp"

uint32_t SCREEN_WIDTH = 1270;
uint32_t SCREEN_HEIGHT = 720;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

Game game(SCREEN_WIDTH, SCREEN_HEIGHT);

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

  game.Init();

  glfwSetKeyCallback(window, key_callback);
  glfwSetScrollCallback(window, scroll_callback);

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);

    float currFrame = glfwGetTime();
    deltaTime = currFrame - lastFrame;
    lastFrame = currFrame;

    game.Update(deltaTime);

    game.HandleInput(deltaTime);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  if (key >= 0 && key < 348)
  {
    if (action == GLFW_PRESS)
      game.Keys[key] = true;
    else if (action == GLFW_RELEASE)
      game.Keys[key] = false;
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  game.ScrollOffset = (float)yoffset;
}
