#pragma once
#include "engine/components/Component.h"
#include "engine/mesh/Mesh.h"
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>

class GameObject; // forward declaration

class MeshComponent : public Component
{
public:
  MeshComponent(std::unique_ptr<Mesh> m);
  ~MeshComponent();

  void Start() override; // Setup GL resources here
  void Draw(unsigned int shader) override;
  glm::vec3 color = glm::vec3(0.5f, 0.0f, 0.5f);

private:
  std::unique_ptr<Mesh> mesh;

  bool meshSetupDone = false;
};
