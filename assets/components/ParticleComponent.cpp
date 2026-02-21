#include "assets/components/ParticleComponent.h"
#include "engine/core/GameObject.h"
#include <glm/glm.hpp>
#include <cmath>

// Virtual destructor definition
ParticleComponent::~ParticleComponent() = default;

// Called once at the start
void ParticleComponent::Start() {
  velocity = glm::vec3(0.0f);
  force = glm::vec3(0.0f);

  if (Owner) {
    prevPosition = Owner->transform.Position;
    Owner->transform.Scale = glm::vec3(radius);
  }
}

// Called every fixed timestep
void ParticleComponent::FixedUpdate(float dt) {
  int subSteps = 5;
  float subDt = dt / subSteps;

  for (int step = 0; step < subSteps; step++) {
    glm::vec3 acceleration = force / mass;
    Owner->transform.Position +=
        velocity * subDt + 0.5f * acceleration * subDt * subDt;
    glm::vec3 oldAcceleration = acceleration;
    glm::vec3 newAcceleration = force / mass;
    velocity += 0.5f * (oldAcceleration + newAcceleration) * subDt;
    velocity *= dampingFactor;

    // Optional containment
    float sphereRadius = 10000.0f;
    if (Owner) {
      glm::vec3& pos = Owner->transform.Position;
      float dist = glm::length(pos);
      if (dist > sphereRadius) {
        glm::vec3 dir = glm::normalize(pos);
        pos = dir * sphereRadius;
        velocity -= glm::dot(velocity, dir) * dir * 1.5f;
      }
    }
  }

  force = glm::vec3(0.0f);
}