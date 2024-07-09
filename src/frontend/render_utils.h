#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void glfw_error_callback(int error, const char* description);
void checkOpenGLError(const std::string &message);
std::string readShaderSource(const std::string& filepath);
unsigned int compileShader(const std::string& source, GLenum type);
unsigned int createShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);