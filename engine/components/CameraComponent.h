#pragma once
#include <glm/glm.hpp>
#include "engine/components/Component.h"
#include "engine/core/GameObject.h"
#include <GLFW/glfw3.h>

enum class CameraType { Perspective, Orthographic };

class CameraComponent : public Component {
 public:
  CameraType Type = CameraType::Perspective;

  float FOV = 45.0f;
  float NearClip = 0.1f;
  float FarClip = 1000.0f;

  float OrthoWidth = 10.0f;
  float OrthoHeight = 10.0f;

  glm::vec3 Up = glm::vec3(0, 1, 0);

  GameObject* Target = nullptr;
  glm::vec3 Offset = glm::vec3(0, 2, 5);

  float Yaw = -90.0f;
  float Pitch = 0.0f;

  CameraComponent() = default;

  glm::mat4 GetViewMatrix() const;
  glm::mat4 GetProjectionMatrix(float aspectRatio) const;
  glm::vec3 GetForward() const;

  void LateUpdate(float dt) override;

  void ProcessInput(GLFWwindow* window, float deltaTime);
  void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch);
};
