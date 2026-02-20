#pragma once
#include "Mesh.h"
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <cmath>

class SphereMesh : public Mesh
{
public:
    SphereMesh(unsigned int XSegments = 32, unsigned int YSegments = 32)
        : xSeg(XSegments), ySeg(YSegments) {}

    void Setup() override
    {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        for (unsigned int y = 0; y <= ySeg; ++y)
        {
            for (unsigned int x = 0; x <= xSeg; ++x)
            {
                float xSegment = (float)x / xSeg;
                float ySegment = (float)y / ySeg;

                float xPos = cos(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
                float yPos = cos(ySegment * M_PI);
                float zPos = sin(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);

                // position
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(zPos);
                // normal
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(zPos);
            }
        }

        for (unsigned int y = 0; y < ySeg; ++y)
        {
            for (unsigned int x = 0; x < xSeg; ++x)
            {
                unsigned int i0 = y * (xSeg + 1) + x;
                unsigned int i1 = i0 + 1;
                unsigned int i2 = i0 + (xSeg + 1);
                unsigned int i3 = i2 + 1;

                indices.push_back(i0);
                indices.push_back(i2);
                indices.push_back(i3);
                indices.push_back(i0);
                indices.push_back(i3);
                indices.push_back(i1);
            }
        }

        indexCount = static_cast<unsigned int>(indices.size());

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
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
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    unsigned int indexCount = 0;
    unsigned int xSeg, ySeg;
};
