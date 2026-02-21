#pragma once
#include <glad.h>

class Mesh {
 public:
  virtual ~Mesh() = default;
  virtual void Draw() = 0;
  virtual void Setup() = 0;
  virtual void Cleanup() = 0;
};
