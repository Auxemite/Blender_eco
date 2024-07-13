#include <vector>
#include "render_utils.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    std::cout << "Viewport set to width: " << width << " height: " << height
              << std::endl;
}

void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void checkOpenGLError(const std::string &message) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error (" << message << "): " << err << std::endl;
    }
}

std::string readShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int compileShader(const std::string& source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    const char* shaderCode = source.c_str();
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
    }

    return shader;
}

unsigned int createShaderProgram(const std::string& vertexPath,
                                 const std::string& fragmentPath,
                                 const std::string& geometryPath) {
    std::string vertexCode = readShaderSource(vertexPath);
    std::string fragmentCode = readShaderSource(fragmentPath);

    unsigned int vertexShader = compileShader(vertexCode, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    if (!geometryPath.empty()) {
        std::string geometryCode = readShaderSource(geometryPath);
        unsigned int geometryShader = compileShader(geometryCode, GL_GEOMETRY_SHADER);
        glAttachShader(shaderProgram, geometryShader);
        glDeleteShader(geometryShader);
    }
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader Program linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}