#pragma once
#include <glm/glm.hpp>

class Entity {
 public:
  Entity();
  virtual ~Entity() = default;

  // Core transform
  glm::vec3 Position;
  glm::vec3 Rotation;  // Euler angles in degrees
  glm::vec3 Scale;

  // Movement
  glm::vec3 Velocity;

  glm::float32 Mass = 1.0f;

  // Update per-frame
  virtual void Update(float deltaTime);

  // Move entity manually
  void Move(const glm::vec3& delta);

  // Apply a force (basic physics)
  void ApplyVelocity(const glm::vec3& velDelta);
};
