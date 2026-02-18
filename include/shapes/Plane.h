#pragma once

class Plane {
 public:
  Plane();
  ~Plane();

  void Draw();
  void setupMesh();
  void cleanup();

 private:
  unsigned int VAO, VBO;
};
