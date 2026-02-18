#include "shapes/Plane.h"
#include <glad/glad.h>

// Simple XZ plane, 1x1 size, centered at origin
float planeVertices[] = {
    // positions         // normals
    -0.5f, 0.0f, -0.5f, 0.0f,  1.0f, 0.0f,  0.5f,  0.0f, -0.5f,
    0.0f,  1.0f, 0.0f,  0.5f,  0.0f, 0.5f,  0.0f,  1.0f, 0.0f,

    0.5f,  0.0f, 0.5f,  0.0f,  1.0f, 0.0f,  -0.5f, 0.0f, 0.5f,
    0.0f,  1.0f, 0.0f,  -0.5f, 0.0f, -0.5f, 0.0f,  1.0f, 0.0f};

Plane::Plane() : VAO(0), VBO(0) {}

Plane::~Plane() { cleanup(); }

void Plane::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices,
               GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void Plane::Draw() {
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);  // 2 triangles for the plane
  glBindVertexArray(0);
}

void Plane::cleanup() {
  if (VBO) glDeleteBuffers(1, &VBO);
  if (VAO) glDeleteVertexArrays(1, &VAO);
  VBO = 0;
  VAO = 0;
}
