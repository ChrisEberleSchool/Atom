#pragma once
#include "engine/components/ParticleComponent.h"
#include "engine/object/GameObject.h"
#include <glm/glm.hpp>
#include <cmath>

void ParticleComponent::Start()
{
    velocity = glm::vec3(0.0f);
    force = glm::vec3(0.0f);

    // Store initial previous position for Verlet (optional, useful for position-only Verlet)
    prevPosition = Owner->Transform.Position;

    // Set initial scale safely
    if (Owner)
        Owner->Transform.Scale = glm::vec3(radius);
}

void ParticleComponent::FixedUpdate(float dt)
{
    int subSteps = 5; // more substeps = more accuracy
    float subDt = dt / subSteps;

    for (int step = 0; step < subSteps; step++)
    {
        // --- Compute acceleration ---
        glm::vec3 acceleration = force / mass;

        // --- Update position using Velocity Verlet ---
        Owner->Transform.Position += velocity * subDt + 0.5f * acceleration * subDt * subDt;

        // Store old acceleration
        glm::vec3 oldAcceleration = acceleration;

        // --- Optional: update forces again if needed after position change ---
        // In your system, forces are probably calculated externally per FixedUpdate.
        // If you want “true substepping”, you would recalc forces here for each substep.
        glm::vec3 newAcceleration = force / mass;

        // --- Update velocity ---
        velocity += 0.5f * (oldAcceleration + newAcceleration) * subDt;

        // Damping for stability
        velocity *= dampingFactor;

        // --- Contain in a sphere ---
        float sphereRadius = 10000.0f;
        if (Owner)
        {
            glm::vec3 &pos = Owner->Transform.Position;
            float dist = glm::length(pos);
            if (dist > sphereRadius)
            {
                glm::vec3 dir = glm::normalize(pos);
                pos = dir * sphereRadius;
                velocity -= glm::dot(velocity, dir) * dir * 1.5f;
            }
        }
    }

    // --- Reset force for next step ---
    force = glm::vec3(0.0f);
}