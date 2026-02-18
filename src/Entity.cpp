#include "Entity.h"

Entity::Entity()
    : Position(0.0f), Rotation(0.0f), Scale(1.0f), Velocity(0.0f) {}

void Entity::Update(float deltaTime) {
  // Simple position update based on velocity
  Position += Velocity * deltaTime;
}

void Entity::Move(const glm::vec3& delta) { Position += delta; }

void Entity::ApplyVelocity(const glm::vec3& velDelta) { Velocity += velDelta; }
