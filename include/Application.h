#pragma once
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "engine/components/CameraComponent.h"
#include "engine/scene/Scene.h"

class Application
{
public:
  Application();
  ~Application();

  void Run();
  void SetActiveScene(std::unique_ptr<Scene> scene);

private:
  std::unique_ptr<Scene> activeScene;

  GLFWwindow *window;
  float lastFrame = 0.0f;
  float deltaTime = 0.0f;

  unsigned int shaderProgram;

  void Init();
  void Shutdown();

  void ProcessInput();
  void Update();
  void FixedUpdate();
  void LateUpdate();
  void Render();

  static void MouseCallback(GLFWwindow *window, double xpos, double ypos);

  // For FPS mouse look
  static bool firstMouse;
  static float lastX;
  static float lastY;
};
