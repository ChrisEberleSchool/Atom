#pragma once
#include "assets/components/ParticleComponent.h"
#include "engine/core/GameObject.h"

class NeutronParticle : public ParticleComponent
{
public:
  NeutronParticle(float radius)
      : ParticleComponent(radius, 1.0f, +0.0f) // radius, mass, charge
  {
  }

  void FixedUpdate(float dt) override
  {
    // Custom proton behavior
    // e.g., maybe protons have stronger repulsion at short distances
    ParticleComponent::FixedUpdate(dt); // call base motion
  }
};