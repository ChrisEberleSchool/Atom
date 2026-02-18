#pragma once

class Cube {
 public:
  Cube();
  ~Cube();

  void Draw();
  void setupMesh();
  void cleanup();

 private:
  unsigned int VAO, VBO;
};
