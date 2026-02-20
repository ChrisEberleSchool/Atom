#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform
{
    glm::vec3 Position{0.0f};
    glm::vec3 Rotation{0.0f};
    glm::vec3 Scale{1.0f};

    glm::mat4 GetModelMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, Position);

        model = glm::rotate(model, glm::radians(Rotation.x), {1, 0, 0});
        model = glm::rotate(model, glm::radians(Rotation.y), {0, 1, 0});
        model = glm::rotate(model, glm::radians(Rotation.z), {0, 0, 1});

        model = glm::scale(model, Scale);
        return model;
    }
};
