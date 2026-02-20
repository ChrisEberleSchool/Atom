#pragma once
#include "ParticleComponent.h"
#include "engine/object/GameObject.h"

class ElectronParticle : public ParticleComponent
{
public:
    ElectronParticle(float radius)
        : ParticleComponent(radius, 0.000544f, -1.0f) // radius, mass, charge
    {
    }

    void FixedUpdate(float dt) override
    {
        // Custom proton behavior
        // e.g., maybe protons have stronger repulsion at short distances
        ParticleComponent::FixedUpdate(dt); // call base motion
    }
};