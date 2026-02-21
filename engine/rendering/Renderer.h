#pragma once
#include <glm/glm.hpp>
#include <string>

class Scene;
class CameraComponent;

class Renderer {
 public:
  Renderer() : activeShader(0) {}
  ~Renderer() {}

  void Init();  // sets up OpenGL state
  unsigned int LoadShader(const std::string& vertex,
                          const std::string& fragment);

  // Set which shader to use for the next draw call
  void UseShader(unsigned int shaderID) { activeShader = shaderID; }

  void DrawScene(Scene* scene, CameraComponent* camera);

 private:
  unsigned int activeShader;  // currently used shader
};