#include "engine/components/MeshComponent.h"
#include "engine/core/GameObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

MeshComponent::MeshComponent(std::unique_ptr<Mesh> m) : mesh(std::move(m)) {
  // Don't call Setup() here
}

void MeshComponent::Start() {
  if (mesh) mesh->Setup();  // OpenGL-safe
}

MeshComponent::~MeshComponent() {
  if (mesh) mesh->Cleanup();
}
void MeshComponent::Draw(unsigned int shader) {
  if (!mesh || !Owner) return;

  // Lazy setup in case Start() was missed
  if (!meshSetupDone) {
    mesh->Setup();
    meshSetupDone = true;
  }

  glm::mat4 model = Owner->transform.GetModelMatrix();
  glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE,
                     glm::value_ptr(model));

  // assign color
  glUniform3f(glGetUniformLocation(shader, "objectColor"), color.x, color.y,
              color.z);

  mesh->Draw();
}
