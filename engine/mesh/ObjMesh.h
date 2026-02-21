#pragma once
#include "Mesh.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glad.h>

class ObjMesh : public Mesh {
 public:
  ObjMesh(const std::string& path) : filePath(path) {}

  void Setup() override {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    std::ifstream file(filePath);
    if (!file.is_open()) {
      std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
      return;
    }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec3> temp_normals;
    std::vector<unsigned int> temp_indices;

    std::string line;
    while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string type;
      ss >> type;

      if (type == "v")  // vertex position
      {
        glm::vec3 pos;
        ss >> pos.x >> pos.y >> pos.z;
        temp_positions.push_back(pos);
      } else if (type == "vn")  // normal
      {
        glm::vec3 norm;
        ss >> norm.x >> norm.y >> norm.z;
        temp_normals.push_back(norm);
      } else if (type == "f")  // face
      {
        unsigned int posIdx[3], normIdx[3];
        char slash;
        for (int i = 0; i < 3; i++) {
          ss >> posIdx[i] >> slash >> slash >> normIdx[i];
          // OBJ indices start at 1
          temp_indices.push_back(posIdx[i] - 1);
        }
      }
    }

    // Flatten vertex data (position + normal)
    for (unsigned int idx : temp_indices) {
      glm::vec3 pos = temp_positions[idx];
      glm::vec3 norm =
          (idx < temp_normals.size()) ? temp_normals[idx] : glm::vec3(0, 1, 0);

      vertices.push_back(pos.x);
      vertices.push_back(pos.y);
      vertices.push_back(pos.z);

      vertices.push_back(norm.x);
      vertices.push_back(norm.y);
      vertices.push_back(norm.z);
    }

    indexCount = static_cast<unsigned int>(vertices.size() / 6);

    // OpenGL buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
  }

  void Draw() override {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, indexCount);
    glBindVertexArray(0);
  }

  void Cleanup() override {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
  }

 private:
  std::string filePath;
  unsigned int VAO = 0, VBO = 0;
  unsigned int indexCount = 0;
};