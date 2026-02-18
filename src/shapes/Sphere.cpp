#include "shapes/Sphere.h"
#include <vector>
#include <cmath>
#include <glad/glad.h>

Sphere::Sphere(unsigned int XSegments, unsigned int YSegments)
    : VAO(0), VBO(0), EBO(0), indexCount(0) {
  // Do not call OpenGL functions here; call setupMesh() after GL context exists
}

Sphere::~Sphere() {}

void Sphere::cleanup() {
  if (VAO) glDeleteVertexArrays(1, &VAO);
  if (VBO) glDeleteBuffers(1, &VBO);
  if (EBO) glDeleteBuffers(1, &EBO);
  VAO = VBO = EBO = 0;
}

void Sphere::setupMesh(unsigned int XSegments, unsigned int YSegments) {
  std::vector<float> vertices;
  std::vector<unsigned int> indices;

  for (unsigned int y = 0; y <= YSegments; ++y) {
    for (unsigned int x = 0; x <= XSegments; ++x) {
      float xSegment = (float)x / XSegments;
      float ySegment = (float)y / YSegments;
      float xPos = std::cos(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI);
      float yPos = std::cos(ySegment * M_PI);
      float zPos = std::sin(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI);

      // position
      vertices.push_back(xPos);
      vertices.push_back(yPos);
      vertices.push_back(zPos);
      // normal (same as position for unit sphere)
      vertices.push_back(xPos);
      vertices.push_back(yPos);
      vertices.push_back(zPos);
    }
  }

  for (unsigned int y = 0; y < YSegments; ++y) {
    for (unsigned int x = 0; x < XSegments; ++x) {
      unsigned int first = y * (XSegments + 1) + x;
      unsigned int second = first + XSegments + 1;
      indices.push_back(first);
      indices.push_back(second);
      indices.push_back(first + 1);

      indices.push_back(second);
      indices.push_back(second + 1);
      indices.push_back(first + 1);
    }
  }

  indexCount = static_cast<unsigned int>(indices.size());

  // Setup OpenGL buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  // vertex positions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // vertex normals
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void Sphere::Draw() {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
