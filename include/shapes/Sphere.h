#pragma once

class Sphere {
 public:
  Sphere(unsigned int XSegments = 32, unsigned int YSegments = 32);
  ~Sphere();

  void Draw();
  void setupMesh(unsigned int XSegments = 32, unsigned int YSegments = 32);
  void cleanup();

 private:
  unsigned int VAO, VBO, EBO;
  unsigned int indexCount;
};
