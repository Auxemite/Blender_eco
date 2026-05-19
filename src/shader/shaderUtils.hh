#pragma once

#include "scene/scene.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

namespace Shader {

    void checkOpenGLError(const std::string &message);
    std::string readShaderSource(const std::string &filepath);
    unsigned int compileShader(const std::string &source, GLenum type, const std::string &string_type);
    unsigned int initShaderProgram(const std::string &vertexPath, const std::string &fragmentPath);
    unsigned int createShaderProgram(const std::string &path);

}