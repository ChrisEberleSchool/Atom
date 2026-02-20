#pragma once
#include "Mesh.h"
#include <glad/glad.h>
#include <vector>

class CubeMesh : public Mesh
{
public:
    CubeMesh(float size = 1.0f) : halfSize(size / 2.0f) {}

    void Setup() override
    {
        float s = halfSize;

        // Vertex format: position(x,y,z) + normal(x,y,z)
        float vertices[] = {
            // Front face
            -s, -s, s, 0, 0, 1, s, -s, s, 0, 0, 1, s, s, s, 0, 0, 1,
            s, s, s, 0, 0, 1, -s, s, s, 0, 0, 1, -s, -s, s, 0, 0, 1,
            // Back face
            -s, -s, -s, 0, 0, -1, s, -s, -s, 0, 0, -1, s, s, -s, 0, 0, -1,
            s, s, -s, 0, 0, -1, -s, s, -s, 0, 0, -1, -s, -s, -s, 0, 0, -1,
            // Left face
            -s, -s, -s, -1, 0, 0, -s, -s, s, -1, 0, 0, -s, s, s, -1, 0, 0,
            -s, s, s, -1, 0, 0, -s, s, -s, -1, 0, 0, -s, -s, -s, -1, 0, 0,
            // Right face
            s, -s, -s, 1, 0, 0, s, -s, s, 1, 0, 0, s, s, s, 1, 0, 0,
            s, s, s, 1, 0, 0, s, s, -s, 1, 0, 0, s, -s, -s, 1, 0, 0,
            // Top face
            -s, s, -s, 0, 1, 0, -s, s, s, 0, 1, 0, s, s, s, 0, 1, 0,
            s, s, s, 0, 1, 0, s, s, -s, 0, 1, 0, -s, s, -s, 0, 1, 0,
            // Bottom face
            -s, -s, -s, 0, -1, 0, -s, -s, s, 0, -1, 0, s, -s, s, 0, -1, 0,
            s, -s, s, 0, -1, 0, s, -s, -s, 0, -1, 0, -s, -s, -s, 0, -1, 0};

        unsigned int indices[36];
        for (unsigned int i = 0; i < 36; i++)
            indices[i] = i;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        indexCount = 36;
    }

    void Draw() override
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Cleanup() override
    {
        if (VAO)
            glDeleteVertexArrays(1, &VAO);
        if (VBO)
            glDeleteBuffers(1, &VBO);
        if (EBO)
            glDeleteBuffers(1, &EBO);
    }

private:
    float halfSize;
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    unsigned int indexCount = 0;
};
