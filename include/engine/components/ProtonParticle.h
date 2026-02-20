#pragma once
#include "ParticleComponent.h"
#include "engine/object/GameObject.h"

class ProtonParticle : public ParticleComponent
{
public:
    ProtonParticle(float radius)
        : ParticleComponent(radius, 1.0f, +1.0f) // radius, mass, charge
    {
    }

    void FixedUpdate(float dt) override
    {
        // Custom proton behavior
        // e.g., maybe protons have stronger repulsion at short distances
        ParticleComponent::FixedUpdate(dt); // call base motion
    }
};