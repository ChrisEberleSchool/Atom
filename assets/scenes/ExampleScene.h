#pragma once
#include "engine/scene/Scene.h"
#include "engine/components/MeshComponent.h"
#include "engine/mesh/SphereMesh.h"
#include "engine/components/CameraComponent.h"
#include "assets/components/ParticleComponent.h"
#include <memory>
#include <vector>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <iostream>

#include "assets/components/ProtonParticle.h"
#include "assets/components/ElectronParticle.h"
#include "assets/components/NeutronParticle.h"

class ExampleScene : public Scene {
 public:
  int protonCount = 20;
  int neutronCount = 20;
  int electronCount = 100;
  float spawnRadius = 20.0f;
  float particleRadius = 1.0f;

  float maxForce = 150.0f;
  float tempStep = 0.5f;
  float currentTemp = 2.0f;

  std::vector<ParticleComponent*> particles;
  std::mt19937 rng{std::random_device{}()};

  // --- Grid parameters ---
  const int gridResolution = 20;
  const float gridSize = 50.0f;  // simulation space from -25 to +25
  std::vector<std::vector<std::vector<float>>> chargeGrid;

  void Start() override {
    auto player = CreateObject();
    player->transform.Position = glm::vec3(0, 0, 40);
    auto cam = player->AddComponent<CameraComponent>();
    cam->Yaw = -90;
    cam->Pitch = 0;
    cam->Offset = glm::vec3(0, 0, 20);

    // initialize empty grid
    chargeGrid.resize(
        gridResolution,
        std::vector<std::vector<float>>(
            gridResolution, std::vector<float>(gridResolution, 0.0f)));

    SpawnParticles();
  }

  static float randf() { return rand() / (float)RAND_MAX; }

  void SpawnParticles() {
    particles.reserve(protonCount + neutronCount + electronCount);

    auto randomVelocity = [this](float maxSpeed) {
      glm::vec3 dir = glm::sphericalRand(1.0f);
      return dir * (randf() * maxSpeed);
    };

    // Protons
    for (int i = 0; i < protonCount; i++) {
      auto obj = CreateObject();
      obj->transform.Position = glm::ballRand(spawnRadius);
      auto mesh =
          obj->AddComponent<MeshComponent>(std::make_unique<SphereMesh>(6, 6));
      mesh->color = glm::vec3(1, 0, 0);
      auto p = obj->AddComponent<ProtonParticle>(particleRadius);
      p->velocity = randomVelocity(0.5f);
      particles.push_back(p);
    }

    // Neutrons
    for (int i = 0; i < neutronCount; i++) {
      auto obj = CreateObject();
      obj->transform.Position = glm::ballRand(spawnRadius);
      auto mesh =
          obj->AddComponent<MeshComponent>(std::make_unique<SphereMesh>(6, 6));
      mesh->color = glm::vec3(0.9f);
      auto n = obj->AddComponent<NeutronParticle>(particleRadius);
      n->velocity = randomVelocity(0.5f);
      particles.push_back(n);
    }

    // Electrons
    for (int i = 0; i < electronCount; i++) {
      auto obj = CreateObject();
      obj->transform.Position = glm::ballRand(spawnRadius);
      auto mesh =
          obj->AddComponent<MeshComponent>(std::make_unique<SphereMesh>(6, 6));
      mesh->color = glm::vec3(0, 0, 1);
      auto e = obj->AddComponent<ElectronParticle>(particleRadius);
      e->velocity = randomVelocity(0.1f);
      particles.push_back(e);
    }

    std::cout << "Spawned " << particles.size() << " particles.\n";
  }

  // Map world position to grid index
  glm::ivec3 WorldToGrid(const glm::vec3& pos) {
    float halfSize = gridSize / 2.0f;
    int x = glm::clamp(int((pos.x + halfSize) / gridSize * gridResolution), 0,
                       gridResolution - 1);
    int y = glm::clamp(int((pos.y + halfSize) / gridSize * gridResolution), 0,
                       gridResolution - 1);
    int z = glm::clamp(int((pos.z + halfSize) / gridSize * gridResolution), 0,
                       gridResolution - 1);
    return glm::ivec3(x, y, z);
  }

  void BuildChargeGrid() {
    // reset
    for (int x = 0; x < gridResolution; x++)
      for (int y = 0; y < gridResolution; y++)
        for (int z = 0; z < gridResolution; z++) chargeGrid[x][y][z] = 0.0f;

    // deposit particle charges into grid
    for (auto* p : particles) {
      glm::ivec3 idx = WorldToGrid(p->Owner->transform.Position);
      chargeGrid[idx.x][idx.y][idx.z] += p->charge;
    }
  }

  void ApplyCoulombGridForces() {
    float k = 5.0f;
    int neighborRange = 1;  // only check immediate neighbors

    for (auto* p : particles) {
      glm::ivec3 idx = WorldToGrid(p->Owner->transform.Position);
      glm::vec3 force(0.0f);

      for (int dx = -neighborRange; dx <= neighborRange; dx++)
        for (int dy = -neighborRange; dy <= neighborRange; dy++)
          for (int dz = -neighborRange; dz <= neighborRange; dz++) {
            int nx = glm::clamp(idx.x + dx, 0, gridResolution - 1);
            int ny = glm::clamp(idx.y + dy, 0, gridResolution - 1);
            int nz = glm::clamp(idx.z + dz, 0, gridResolution - 1);

            float q = chargeGrid[nx][ny][nz];
            if (q == 0.0f) continue;

            // approximate cell center position
            float cellSize = gridSize / gridResolution;
            glm::vec3 cellPos((nx + 0.5f) * cellSize - gridSize / 2.0f,
                              (ny + 0.5f) * cellSize - gridSize / 2.0f,
                              (nz + 0.5f) * cellSize - gridSize / 2.0f);

            glm::vec3 delta = cellPos - p->Owner->transform.Position;
            float dist = glm::length(delta);
            if (dist < 0.5f) dist = 0.5f;

            force +=
                glm::normalize(delta) * (k * p->charge * q) / (dist * dist);
          }

      // clamp and apply
      float fMag = glm::length(force);
      if (fMag > maxForce) force = force / fMag * maxForce;
      p->AddForce(force);
    }
  }

  void calculateStrongForce(ParticleComponent* a, ParticleComponent* b) {
    if (dynamic_cast<ElectronParticle*>(a) ||
        dynamic_cast<ElectronParticle*>(b))
      return;

    glm::vec3 delta =
        b->Owner->transform.Position - a->Owner->transform.Position;
    float dist = glm::length(delta);
    glm::vec3 dir = delta / dist;

    float attraction = 500.0f;
    float wellDist = 0.5f;
    float minDist = 0.65f;

    float forceMag = 1.0f / glm::pow(dist, 12) -
                     attraction / dist * glm::exp(-dist / wellDist);

    if (dist < minDist)
      forceMag = glm::clamp(forceMag, -maxForce / 2.0f, maxForce / 2.0f);
    else
      forceMag = glm::clamp(forceMag, -maxForce, maxForce);

    glm::vec3 force = dir * forceMag;
    a->AddForce(-force);
    b->AddForce(force);
  }

  void AddTemp(float tempStrength) {
    for (auto* p : particles) {
      if (dynamic_cast<ProtonParticle*>(p) ||
          dynamic_cast<NeutronParticle*>(p)) {
        glm::vec3 kick = glm::sphericalRand(1.0f) * tempStrength;
        p->velocity += kick;
      }
    }
  }

  void HandleInput(GLFWwindow* window) override {
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
      currentTemp += tempStep;
      std::cout << "Temperature: " << currentTemp << "\n";
    }

    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
      currentTemp = glm::max(0.0f, currentTemp - tempStep);
      std::cout << "Temperature: " << currentTemp << "\n";
    }
  }

  void FixedUpdate(float dt) override {
    for (auto* p : particles) p->force = glm::vec3(0.0f);

    // Strong nuclear force
    for (size_t i = 0; i < particles.size(); i++) {
      for (size_t j = i + 1; j < particles.size(); j++) {
        calculateStrongForce(particles[i], particles[j]);
      }
    }

    // Build charge grid and apply approximate Coulomb forces
    BuildChargeGrid();
    ApplyCoulombGridForces();

    AddTemp(currentTemp);

    Scene::FixedUpdate(dt);
  }
};