#pragma once

#include "glad/gl.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

namespace Shader {

void checkOpenGLError(const std::string &message);
std::string readShaderSource(const std::string &filepath);
unsigned int compileShader(const std::string &source, GLenum type, const std::string &string_type);

unsigned int initShaderProgram();
unsigned int createShaderProgram(const std::string &path);

}