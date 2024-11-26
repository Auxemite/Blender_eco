#include <vector>
#include "shader_utils.hh"

namespace fs = std::filesystem;

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

unsigned int compileShader(const std::string& source, GLenum type, const std::string& string_type) {
    unsigned int shader = glCreateShader(type);
    const char* shaderCode = source.c_str();
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation " << string_type << " failed:\n" << infoLog << std::endl;
    }

    return shader;
}

unsigned int invalidShaderPath(const std::string& path) {
    if (!fs::exists(path)) {
        std::cerr << "verifyShaderPath : Path does not exist" << std::endl;
        return 1;
    }
    if (!fs::is_directory(path)) {
        std::cerr << "verifyShaderPath : Path is not a directory" << std::endl;
        return 1;
    }

    int shader_nb[5] = {0, 0, 0, 0, 0};
    for (const auto & entry : fs::directory_iterator(path))
    {
        if (entry.path().extension() == ".vert")
            shader_nb[0]++;
        if (entry.path().extension() == ".frag")
            shader_nb[1]++;
        if (entry.path().extension() == ".geom")
            shader_nb[2]++;
        if (entry.path().extension() == ".tesc")
            shader_nb[3]++;
        if (entry.path().extension() == ".tese")
            shader_nb[4]++;
    }
    if (shader_nb[0] != 1 && shader_nb[1] != 1)
    {
        std::cerr << "verifyShaderPath : Found more than one shader of type vtx or frag" << std::endl;
        return 1;
    }
    for (int i = 2; i < 5; ++i) {
        if (shader_nb[i] > 1) {
            std::cerr << "verifyShaderPath : Found more than one shader of type " << i << std::endl;
            return 1;
        }
    }

    if (shader_nb[3] != shader_nb[4]) {
        std::cerr << "createShaderProgram : Found only one of the two Tesselation Shaders" << std::endl;
        return 1;
    }
    return 0;
}

unsigned int createShaderProgram(const std::string& path) {
    if (invalidShaderPath(path))
    {
        std::cerr << "createShaderProgram : Failed to read shader source files" << std::endl;
        return 0;
    }
    std::string vertexCode = "";
    std::string fragmentCode = "";
    std::string geometryCode = "";
    std::string tessControlCode = "";
    std::string tessEvalCode = "";
    for (const auto & entry : fs::directory_iterator(path))
    {
        if (entry.path().extension() == ".vert")
            vertexCode = readShaderSource(entry.path().string());
        if (entry.path().extension() == ".frag")
            fragmentCode = readShaderSource(entry.path().string());
        if (entry.path().extension() == ".geom")
            geometryCode = readShaderSource(entry.path().string());
        if (entry.path().extension() == ".tesc")
            tessControlCode = readShaderSource(entry.path().string());
        if (entry.path().extension() == ".tese")
            tessEvalCode = readShaderSource(entry.path().string());
    }

    unsigned int vertexShader = compileShader(vertexCode, GL_VERTEX_SHADER, "Vertex");
    unsigned int fragmentShader = compileShader(fragmentCode, GL_FRAGMENT_SHADER, "Fragment");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    if (!geometryCode.empty()) {
        unsigned int geometryShader = compileShader(geometryCode, GL_GEOMETRY_SHADER, "Geometry");
        glAttachShader(shaderProgram, geometryShader);
        glDeleteShader(geometryShader);
        checkOpenGLError("Geometry control shader Error");
    }
    if (!tessControlCode.empty()) {
        unsigned int tessControlShader = compileShader(tessControlCode, GL_TESS_CONTROL_SHADER, "Tesselation Control");
        glAttachShader(shaderProgram, tessControlShader);
        glDeleteShader(tessControlShader);
        checkOpenGLError("Tesselation control shader Error");
    }
    if (!tessEvalCode.empty()) {
        unsigned int tessEvalShader = compileShader(tessEvalCode, GL_TESS_EVALUATION_SHADER, "Tesselation Evaluation");
        glAttachShader(shaderProgram, tessEvalShader);
        glDeleteShader(tessEvalShader);
        checkOpenGLError("Tesselation Evaluation shader Error");
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