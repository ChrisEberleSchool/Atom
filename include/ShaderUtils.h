#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

inline std::string readShaderFile(const char* path) {
  std::ifstream file(path);
  std::stringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

inline unsigned int compileShader(const char* path, GLenum type) {
  std::string code = readShaderFile(path);
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

inline unsigned int createShaderProgram(const char* vertexPath,
                                        const char* fragmentPath) {
  unsigned int vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
  unsigned int fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}
