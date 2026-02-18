#pragma once
#include "Entity.h"
#include "MeshEntity.h"
#include "camera/Camera.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

class Application {
 public:
  Application();
  ~Application();

  void Run();

 private:
  GLFWwindow* window;
  Camera camera;
  float lastFrame;
  float deltaTime;

  unsigned int shaderProgram;
  std::vector<std::unique_ptr<MeshEntity>> entities;

  void Init();
  void Update();
  void Render();
  void Shutdown();

  static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

  float fpsTimer = 0.0f;
  int fpsFrames = 0;
  float currentFPS = 0.0f;
};
