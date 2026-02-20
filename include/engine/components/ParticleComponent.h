#pragma once
#include "engine/components/Component.h"
#include <glm/glm.hpp>
#include <cstdlib>

class GameObject; // forward declaration, okay for pointer

class ParticleComponent : public Component
{
public:
    ParticleComponent(float r = 1.0f, float m = 1.0f, float q = 1.0f)
        : radius(r), mass(m), charge(q) {}

    virtual void Start() override;
    virtual void FixedUpdate(float dt) override;

    // ---- Force Functions ----
    void AddForce(const glm::vec3 &f) { force += f; }
    void ResetForce() { force = glm::vec3(0.0f); }
    glm::vec3 GetForce() const { return force; }
    void SetForce(const glm::vec3 &f) { force = f; }

    // --- Motion ---
    glm::vec3 velocity{0.0f};
    glm::vec3 force{0.0f};
    float dampingFactor = 0.9f;

    // --- Particle properties ---
    float radius{1.0f};
    float mass{1.0f};
    float charge{0.0f};

    glm::vec3 prevPosition;
};