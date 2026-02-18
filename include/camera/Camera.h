#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
 public:
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;

  float Yaw;
  float Pitch;
  float MovementSpeed;
  float MouseSensitivity;

  bool firstMouse;
  float lastX, lastY;

  Camera(glm::vec3 startPos, float startYaw = -90.0f, float startPitch = 0.0f);

  glm::mat4 GetViewMatrix();

  void ProcessKeyboard(GLFWwindow* window, float deltaTime);
  void ProcessMouseMovement(double xpos, double ypos);
};
