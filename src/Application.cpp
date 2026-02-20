#include "Application.h"
#include "ShaderUtils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "engine/components/CameraComponent.h"

bool Application::firstMouse = true;
float Application::lastX = 640.0f;
float Application::lastY = 360.0f;

Application::Application() : window(nullptr), shaderProgram(0) {}
Application::~Application() { Shutdown(); }

void Application::Init()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(1280, 720, "Atom Engine", nullptr, nullptr);
  if (!window)
  {
    std::cerr << "Failed to create window\n";
    exit(-1);
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD\n";
    exit(-1);
  }

  glEnable(GL_DEPTH_TEST);

  shaderProgram = createShaderProgram("../shaders/basic.vert", "../shaders/basic.frag");

  glfwSetWindowUserPointer(window, this);
  glfwSetCursorPosCallback(window, MouseCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::SetActiveScene(std::unique_ptr<Scene> scene)
{
  activeScene = std::move(scene);
}

void Application::ProcessInput()
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowShouldClose(window, true);
  }

  // Let the active camera process movement
  if (activeScene)
  {
    for (auto &obj : activeScene->GetObjects())
    {
      for (auto *comp : obj->GetComponents())
      {
        CameraComponent *cam = dynamic_cast<CameraComponent *>(comp);
        if (cam)
        {
          cam->ProcessInput(window, deltaTime);
          break;
        }
      }
    }
    activeScene->HandleInput(window);
  }
}

void Application::Update()
{
  if (activeScene)
    activeScene->Update(deltaTime);
}

void Application::FixedUpdate()
{
  if (activeScene)
    activeScene->FixedUpdate(1.0f / 60.0f);
}

void Application::LateUpdate()
{
  if (activeScene)
    activeScene->LateUpdate(deltaTime);
}

void Application::Render()
{
  glClearColor(0.1f, 0.1f, 0.1f, 0.70f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shaderProgram);

  // --- Setup light ---
  glm::vec3 lightDir = glm::normalize(glm::vec3(-0.3f, -1.0f, -0.5f));
  glUniform3f(glGetUniformLocation(shaderProgram, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
  glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f); // white light

  // --- Get active camera ---
  CameraComponent *camera = nullptr;
  if (activeScene)
  {
    for (auto &obj : activeScene->GetObjects())
    {
      for (auto *comp : obj->GetComponents())
      {
        camera = dynamic_cast<CameraComponent *>(comp);
        if (camera)
          break;
      }
      if (camera)
        break;
    }
  }

  glm::mat4 view(1.0f), projection(1.0f);
  if (camera)
  {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = static_cast<float>(width) / height;

    view = camera->GetViewMatrix();
    projection = camera->GetProjectionMatrix(aspect);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
  }

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // --- Draw all scene objects ---
  if (activeScene)
    activeScene->Draw(shaderProgram);
}

void Application::MouseCallback(GLFWwindow *window, double xpos, double ypos)
{
  Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
  if (!app)
    return;

  // Get active camera
  CameraComponent *camera = nullptr;
  if (app->activeScene)
  {
    for (auto &obj : app->activeScene->GetObjects())
    {
      for (auto *comp : obj->GetComponents())
      {
        camera = dynamic_cast<CameraComponent *>(comp);
        if (camera)
          break;
      }
      if (camera)
        break;
    }
  }
  if (!camera)
    return;

  static bool firstMouse = true;
  static float lastX = 640.0f;
  static float lastY = 360.0f;

  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed
  lastX = xpos;
  lastY = ypos;

  camera->ProcessMouseMovement(xoffset, yoffset, true); // constrainPitch = true
}

void Application::Run()
{
  Init(); // creates OpenGL context and loads GLAD

  // Now safe to start scene objects
  if (activeScene)
    activeScene->Start();

  float accumulator = 0.0f;

  while (!glfwWindowShouldClose(window))
  {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    accumulator += deltaTime;

    ProcessInput();

    while (accumulator >= 1.0f / 60.0f)
    {
      FixedUpdate();
      accumulator -= 1.0f / 60.0f;
    }

    Update();
    LateUpdate();
    Render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  Shutdown();
}

void Application::Shutdown()
{
  activeScene.reset();
  glfwTerminate();
}
