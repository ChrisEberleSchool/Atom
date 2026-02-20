#include "engine/components/CameraComponent.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

glm::mat4 CameraComponent::GetViewMatrix() const
{
    glm::vec3 position = Owner->Transform.Position;
    glm::vec3 targetPos = Target ? Target->Transform.Position : position + GetForward();
    return glm::lookAt(position, targetPos, Up);
}

glm::mat4 CameraComponent::GetProjectionMatrix(float aspectRatio) const
{
    if (Type == CameraType::Perspective)
        return glm::perspective(glm::radians(FOV), aspectRatio, NearClip, FarClip);
    else
        return glm::ortho(-OrthoWidth / 2, OrthoWidth / 2, -OrthoHeight / 2, OrthoHeight / 2, NearClip, FarClip);
}

glm::vec3 CameraComponent::GetForward() const
{
    glm::vec3 dir;
    dir.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    dir.y = sin(glm::radians(Pitch));
    dir.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    return glm::normalize(dir);
}

void CameraComponent::LateUpdate(float dt)
{
    if (Target)
        Owner->Transform.Position = Target->Transform.Position + Offset;
}

// ---- WASD / vertical movement ----
void CameraComponent::ProcessInput(GLFWwindow *window, float deltaTime)
{
    if (!Owner)
        return;

    float velocity = 60.0f * deltaTime;

    if (!Target)
    {
        glm::vec3 forward = GetForward();
        glm::vec3 right = glm::normalize(glm::cross(forward, Up));

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            Owner->Transform.Position += forward * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Owner->Transform.Position -= forward * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Owner->Transform.Position -= right * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Owner->Transform.Position += right * velocity;

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            Owner->Transform.Position += Up * velocity;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            Owner->Transform.Position -= Up * velocity;
    }
}

// ---- Mouse look ----
void CameraComponent::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
}
