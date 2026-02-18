#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Application.h"
#include "ShaderUtils.h"
#include "shapes/Cube.h"
#include "shapes/Sphere.h"

// Static camera pointer for mouse callbacks
static Camera* s_camera = nullptr;

Application::Application()
    : camera(glm::vec3(0.0f, 1.0f, 5.0f)), lastFrame(0.0f), deltaTime(0.0f) {}

Application::~Application() {
  // entities will be automatically cleaned up here while OpenGL context is
  // alive
}

std::string ReadFile(const char* path) {
  std::ifstream file(path);
  std::stringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

unsigned int CompileShader(const char* path, GLenum type) {
  std::string code = ReadFile(path);
  const char* c_code = code.c_str();
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &c_code, NULL);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  return shader;
}

unsigned int CreateShaderProgram(const char* vertex, const char* fragment) {
  unsigned int v = CompileShader(vertex, GL_VERTEX_SHADER);
  unsigned int f = CompileShader(fragment, GL_FRAGMENT_SHADER);

  unsigned int program = glCreateProgram();
  glAttachShader(program, v);
  glAttachShader(program, f);
  glLinkProgram(program);

  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(v);
  glDeleteShader(f);
  return program;
}
#include "Constants.h"
void Application::Init() {
  // GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(1280, 720, "Atom", nullptr, nullptr);
  if (!window) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(window);

  // GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    exit(-1);
  }

  glEnable(GL_DEPTH_TEST);

  // Shader
  shaderProgram =
      CreateShaderProgram("../shaders/basic.vert", "../shaders/basic.frag");

  // --- STAR ---
  entities.push_back(std::make_unique<MeshEntity>(ShapeType::Sphere, 5.0f));
  MeshEntity* star = entities.back().get();
  star->Position = glm::vec3(0.0f);
  star->Mass = 1e10f;
  star->Velocity = glm::vec3(0.0f);

  // --- PLANET 1 ---
  entities.push_back(std::make_unique<MeshEntity>(ShapeType::Sphere, 1.0f));
  MeshEntity* planet1 = entities.back().get();
  planet1->Position = glm::vec3(10.0f, 0.0f, 0.0f);
  planet1->Mass = 1.0f;
  float v1 = sqrt(G * star->Mass * 2.0f);
  planet1->Velocity = glm::vec3(0.0f, 0.0f, v1);

  // --- PLANET 2 ---
  entities.push_back(std::make_unique<MeshEntity>(ShapeType::Sphere, 1.5f));
  MeshEntity* planet2 = entities.back().get();
  planet2->Position = glm::vec3(-15.0f, 0.0f, 0.0f);
  planet2->Mass = 2.0f;
  float v2 = sqrt(G * star->Mass * 5.0f);
  planet2->Velocity = glm::vec3(0.0f, 0.0f, -v2);

  // --- PLANET 3 ---
  entities.push_back(std::make_unique<MeshEntity>(ShapeType::Sphere, 0.8f));
  MeshEntity* planet3 = entities.back().get();
  planet3->Position = glm::vec3(0.0f, 0.0f, 20.0f);
  planet3->Mass = 0.5f;
  float v3 = sqrt(G * star->Mass * 3.0f);
  planet3->Velocity = glm::vec3(-v3, 0.0f, 0.0f);

  // entities.push_back(std::make_unique<MeshEntity>(ShapeType::Plane, 100.0f));
  // entities.back()->Position = glm::vec3(0.0f, 0.0f, 0.0f);

  // Mouse
  glfwSetCursorPosCallback(window, MouseCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Static camera pointer for callbacks
  s_camera = &camera;
}

void Application::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
  if (s_camera) s_camera->ProcessMouseMovement(xpos, ypos);
}

void HandleSphereCollisions(
    std::vector<std::unique_ptr<MeshEntity>>& entities) {
  for (size_t i = 0; i < entities.size(); ++i) {
    for (size_t j = i + 1; j < entities.size(); ++j) {
      MeshEntity* a = entities[i].get();
      MeshEntity* b = entities[j].get();

      if (a->Type != ShapeType::Sphere || b->Type != ShapeType::Sphere)
        continue;

      float distance = glm::length(a->Position - b->Position);
      float combinedRadius = a->Size + b->Size;

      if (distance < combinedRadius) {
        // Determine survivor and smaller sphere
        MeshEntity* survivor = (a->Mass >= b->Mass) ? a : b;
        MeshEntity* toDelete = (a->Mass < b->Mass) ? a : b;

        // Add mass
        survivor->Mass += toDelete->Mass;

        // Adjust velocity using momentum conservation
        survivor->Velocity = (survivor->Mass * survivor->Velocity +
                              toDelete->Mass * toDelete->Velocity) /
                             survivor->Mass;

        // Update size based on mass (assuming Size ∝ cube root of Mass)
        survivor->Size += toDelete->Size;
        // Delete the smaller sphere
        for (auto it = entities.begin(); it != entities.end(); ++it) {
          if (it->get() == toDelete) {
            entities.erase(it);
            break;
          }
        }

        // Vector changed, break inner loop
        break;
      }
    }
  }
}

void ApplyGravForce(Entity& entity0, Entity& entity1) {
  glm::vec3 direction = entity1.Position - entity0.Position;
  float distance = glm::length(direction);
  if (distance < 0.01f) return;  // Avoid singularity

  glm::vec3 forceDir = glm::normalize(direction);
  float forceMag = G * (entity0.Mass * entity1.Mass) / (distance * distance);
  glm::vec3 force = forceDir * forceMag;

  // Apply only to entity0
  entity0.ApplyVelocity(force / entity0.Mass);
  // entity1 will have its own force applied in another iteration
}

void Application::Update() {
  float currentFrame = (float)glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;

  camera.ProcessKeyboard(window, deltaTime);

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // Example: resize first entity
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) entities[0]->Size += 0.1f;
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) entities[0]->Size -= 0.1f;

  // Apply gravity between all pairs safely
  for (size_t i = 0; i < entities.size(); ++i) {
    for (size_t j = 0; j < entities.size(); ++j) {
      if (i == j) continue;
      ApplyGravForce(*entities[i], *entities[j]);
    }
  }

  // Update all entities
  for (auto& e : entities) e->Update(deltaTime);

  // --- Handle collisions between spheres ---
  HandleSphereCollisions(entities);

  // ----------------------
  // FPS Counter
  // ----------------------
  fpsFrames++;
  fpsTimer += deltaTime;

  if (fpsTimer >= 1.0f) {  // update once per second
    currentFPS = fpsFrames / fpsTimer;
    fpsFrames = 0;
    fpsTimer = 0.0f;

    // Update GLFW window title
    std::string title =
        "Cube + Sphere - FPS: " + std::to_string((int)currentFPS);
    glfwSetWindowTitle(window, title.c_str());
  }
}

void Application::Render() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shaderProgram);

  // View and projection
  glm::mat4 view = camera.GetViewMatrix();
  glm::mat4 projection =
      glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE,
                     glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1,
                     GL_FALSE, glm::value_ptr(projection));

  // Directional light from "high in the sky"
  glm::vec3 lightDir = glm::normalize(glm::vec3(-0.3f, -1.0f, -0.5f));
  glUniform3f(glGetUniformLocation(shaderProgram, "lightDir"), lightDir.x,
              lightDir.y, lightDir.z);

  // Make it brighter
  glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.1f, 1.0f,
              1.1f);

  // Draw all entities
  for (auto& e : entities) {
    glm::vec3 color;
    if (e->Type == ShapeType::Cube)
      color = glm::vec3(1.0f, 0.5f, 0.31f);
    else if (e->Type == ShapeType::Sphere)
      color = glm::vec3(0.2f, 0.7f, 0.3f);
    else
      color = glm::vec3(0.6f, 0.6f, 0.6f);  // Plane

    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), color.r,
                color.g, color.b);

    e->Draw(shaderProgram);
  }
}

void Application::Run() {
  Init();
  while (!glfwWindowShouldClose(window)) {
    Update();
    Render();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  Shutdown();
}

void Application::Shutdown() {
  // Clear all entities while OpenGL context is still alive
  entities.clear();
  glfwTerminate();
}
