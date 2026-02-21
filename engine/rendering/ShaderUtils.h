#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Reads the contents of a text file and returns it as a string
inline std::string readFile(const char *path)
{
  std::ifstream file(path);
  if (!file.is_open())
  {
    std::cerr << "Failed to open file: " << path << std::endl;
    return "";
  }

  std::stringstream ss;
  ss << file.rdbuf();
  return ss.str();
}

// Compiles a shader from a file and returns the shader ID
inline unsigned int compileShader(const char *path, GLenum type)
{
  std::string code = readFile(path);
  const char *cCode = code.c_str();

  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &cCode, nullptr);
  glCompileShader(shader);

  // Check for compilation errors
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cerr << "Shader compilation failed (" << path << "):\n"
              << infoLog << std::endl;
  }

  return shader;
}

// Creates a shader program from vertex and fragment shader files
inline unsigned int createShaderProgram(const char *vertexPath,
                                        const char *fragmentPath)
{
  unsigned int vertexShader = compileShader(vertexPath, GL_VERTEX_SHADER);
  unsigned int fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

  unsigned int program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  // Check for linking errors
  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    std::cerr << "Shader program linking failed:\n"
              << infoLog << std::endl;
  }

  // Delete shaders after linking
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}
