#include "MeshEntity.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

MeshEntity::MeshEntity(ShapeType type, float size) : Type(type), Size(size) {
  if (Type == ShapeType::Cube) {
    cube.setupMesh();
  } else if (Type == ShapeType::Sphere) {
    sphere.setupMesh(32, 32);
  } else if (Type == ShapeType::Plane) {
    plane.setupMesh();
  }

  this->Mass = Size * 90000000000000.0f;  // Arbitrary mass based on size
}

MeshEntity::~MeshEntity() {
  cube.cleanup();
  sphere.cleanup();
  plane.cleanup();
}

void MeshEntity::Update(float deltaTime) {
  // Update position using velocity
  Entity::Update(deltaTime);
}

void MeshEntity::Draw(unsigned int shaderProgram) {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, Position);
  model = glm::scale(model, glm::vec3(Size));
  // TODO: add rotation if you want
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE,
                     glm::value_ptr(model));

  // Assign a simple color
  if (Type == ShapeType::Cube)
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 1.0f, 0.5f,
                0.31f);
  else if (Type == ShapeType::Sphere)
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.2f, 0.7f,
                0.3f);
  else if (Type == ShapeType::Plane)
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.6f, 0.6f,
                0.6f);  // gray for ground

  // Draw the correct mesh
  if (Type == ShapeType::Cube)
    cube.Draw();
  else if (Type == ShapeType::Sphere)
    sphere.Draw();
  else if (Type == ShapeType::Plane)
    plane.Draw();
}
