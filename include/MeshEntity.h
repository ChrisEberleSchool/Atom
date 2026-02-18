#pragma once
#include "Entity.h"
#include "shapes/Cube.h"
#include "shapes/Plane.h"
#include "shapes/Sphere.h"
#include <memory>
#include <string>
#include "shapes/ShapeType.h"

class MeshEntity : public Entity {
 public:
  MeshEntity(ShapeType type, float size = 1.0f);

  ~MeshEntity();

  void Draw(unsigned int shaderProgram);
  void Update(float deltaTime) override;

  ShapeType Type;
  float Size;

 private:
  Cube cube;
  Sphere sphere;
  Plane plane;
};
