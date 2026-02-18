#include "camera/Camera.h"
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(glm::vec3 startPos, float startYaw, float startPitch)
    : Position(startPos),
      Front(0.0f, 0.0f, -1.0f),
      Up(0.0f, 1.0f, 0.0f),
      Yaw(startYaw),
      Pitch(startPitch),
      MovementSpeed(5.0f),
      MouseSensitivity(0.1f),
      firstMouse(true) {
  lastX = 1280 / 2.0f;
  lastY = 720 / 2.0f;
}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(GLFWwindow* window, float deltaTime) {
  float speed = MovementSpeed * deltaTime;
  glm::vec3 right = glm::normalize(glm::cross(Front, Up));

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) Position += speed * Front;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) Position -= speed * Front;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) Position -= speed * right;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) Position += speed * right;
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) Position += speed * Up;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    Position -= speed * Up;
}

void Camera::ProcessMouseMovement(double xpos, double ypos) {
  if (firstMouse) {
    lastX = (float)xpos;
    lastY = (float)ypos;
    firstMouse = false;
  }

  float xoffset = (float)xpos - lastX;
  float yoffset = lastY - (float)ypos;  // reversed
  lastX = (float)xpos;
  lastY = (float)ypos;

  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw += xoffset;
  Pitch += yoffset;

  if (Pitch > 89.0f) Pitch = 89.0f;
  if (Pitch < -89.0f) Pitch = -89.0f;

  Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front.y = sin(glm::radians(Pitch));
  Front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(Front);
}
