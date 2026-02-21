#include "Renderer.h"
#include "ShaderUtils.h"
#include "engine/scene/Scene.h"
#include "engine/components/CameraComponent.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

void Renderer::Init()
{
  glEnable(GL_DEPTH_TEST);
  // Load a default engine shader for fallback
  activeShader = LoadShader("../engine/rendering/shaders/basic.vert",
                            "../engine/rendering/shaders/basic.frag");
}

unsigned int Renderer::LoadShader(const std::string &v, const std::string &f)
{
  return createShaderProgram(v.c_str(), f.c_str());
}

void Renderer::DrawScene(Scene *scene, CameraComponent *camera)
{
  if (!scene || !camera || !activeShader)
    return;

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(activeShader);

  int width, height;
  glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
  float aspect = float(width) / height;

  glm::mat4 view = camera->GetViewMatrix();
  glm::mat4 projection = camera->GetProjectionMatrix(aspect);

  glUniformMatrix4fv(glGetUniformLocation(activeShader, "view"), 1, GL_FALSE,
                     glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(activeShader, "projection"), 1,
                     GL_FALSE, glm::value_ptr(projection));

  scene->Draw(activeShader);
}