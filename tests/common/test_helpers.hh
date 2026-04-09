#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "glad/gl.h"
#include "gui/window.hh"

namespace TestHelpers {

class ScopedWindowContext {
  public:
    ScopedWindowContext() : window_(Window::softwareContextInit()) {}

    ~ScopedWindowContext() {
        if (window_ != nullptr) {
            Window::shutDown(window_);
        }
    }

    ScopedWindowContext(const ScopedWindowContext&) = delete;
    ScopedWindowContext& operator=(const ScopedWindowContext&) = delete;

    GLFWwindow* window() const { return window_; }
    bool valid() const { return window_ != nullptr; }

  private:
    GLFWwindow* window_ = nullptr;
};

inline std::filesystem::path repoRoot() {
    std::filesystem::path current = std::filesystem::current_path();
    for (int i = 0; i < 8; ++i) {
        if (std::filesystem::exists(current / "CMakeLists.txt")) {
            return current;
        }
        if (!current.has_parent_path()) {
            break;
        }
        current = current.parent_path();
    }
    return std::filesystem::current_path();
}

inline std::string dataFile(const std::string& fileName) {
    return (repoRoot() / "data" / fileName).string();
}

inline std::filesystem::path createTempShaderFile(const std::filesystem::path& dir,
                                                  const std::string& fileName,
                                                  const std::string& source) {
    std::filesystem::create_directories(dir);
    std::filesystem::path filePath = dir / fileName;
    std::ofstream out(filePath);
    out << source;
    out.close();
    return filePath;
}

inline unsigned int createMinimalProgram() {
    const char* vertexSource =
            "#version 450 core\n"
            "layout(location = 0) in vec3 aPos;\n"
            "void main(){ gl_Position = vec4(aPos, 1.0); }\n";

    const char* fragmentSource =
            "#version 450 core\n"
            "out vec4 FragColor;\n"
            "void main(){ FragColor = vec4(1.0); }\n";

    const unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexSource, nullptr);
    glCompileShader(vs);

    const unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentSource, nullptr);
    glCompileShader(fs);

    const unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

} // namespace TestHelpers
