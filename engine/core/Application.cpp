#include "Application.h"
#include <iostream>

bool Application::firstMouse = true;
float Application::lastX = 640.0f;
float Application::lastY = 360.0f;

Application::Application() {}
Application::~Application() { Shutdown(); }

void Application::Init() {
  // --- GLFW Window setup ---
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(1280, 720, "Atom Engine", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create window\n";
    exit(-1);
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    exit(-1);
  }

  glfwSetWindowUserPointer(window, this);
  glfwSetCursorPosCallback(window, MouseCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // --- Renderer initialization ---
  renderer.Init();
}

void Application::SetActiveScene(std::unique_ptr<Scene> scene) {
  activeScene = std::move(scene);
}

void Application::ProcessInput() {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowShouldClose(window, true);
  }

  if (!activeScene) return;

  for (auto& obj : activeScene->GetObjects()) {
    for (auto* comp : obj->GetComponents()) {
      if (CameraComponent* cam = dynamic_cast<CameraComponent*>(comp)) {
        cam->ProcessInput(window, deltaTime);
        break;
      }
    }
  }

  activeScene->HandleInput(window);
}

void Application::Update() {
  if (activeScene) activeScene->Update(deltaTime);
}

void Application::FixedUpdate() {
  if (activeScene) activeScene->FixedUpdate(1.0f / 60.0f);
}

void Application::LateUpdate() {
  if (activeScene) activeScene->LateUpdate(deltaTime);
}

void Application::Render() {
  CameraComponent* camera = nullptr;
  if (activeScene) {
    for (auto& obj : activeScene->GetObjects()) {
      for (auto* comp : obj->GetComponents()) {
        camera = dynamic_cast<CameraComponent*>(comp);
        if (camera) break;
      }
      if (camera) break;
    }
  }

  renderer.DrawScene(activeScene.get(), camera);
}

void Application::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
  Application* app =
      static_cast<Application*>(glfwGetWindowUserPointer(window));
  if (!app) return;

  CameraComponent* camera = nullptr;
  if (app->activeScene) {
    for (auto& obj : app->activeScene->GetObjects()) {
      for (auto* comp : obj->GetComponents()) {
        camera = dynamic_cast<CameraComponent*>(comp);
        if (camera) break;
      }
      if (camera) break;
    }
  }
  if (!camera) return;

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;  // reversed
  lastX = xpos;
  lastY = ypos;

  camera->ProcessMouseMovement(xoffset, yoffset, true);
}

void Application::Run() {
  Init();

  if (activeScene) activeScene->Start();

  float accumulator = 0.0f;

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    accumulator += deltaTime;

    ProcessInput();

    while (accumulator >= 1.0f / 60.0f) {
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

void Application::Shutdown() {
  activeScene.reset();
  glfwTerminate();
}