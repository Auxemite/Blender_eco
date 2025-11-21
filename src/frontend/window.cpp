#include "window.hh"

#include <iostream>

namespace Window {

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    std::cout << "Viewport set to width: " << width << " height: " << height
              << std::endl;
}

void glfwErrorCallback(int error, const char *description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

GLFWwindow *glfwWindowInit() {
    if (!glfwInit()) {
        std::cerr << "Error glfw_window_init : Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1920, 1080, "Blender Eco+", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error glfw_window_init : Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Window::framebufferSizeCallback);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Error glfw_window_init : Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    return window;
}

void processInpute(GLFWwindow *window, Camera *camera) {
    float cameraSpeedz = camera->speed_zoom * Env::deltaTime;
    float cameraSpeedr = camera->speed_rotation * 10.0f * Env::deltaTime;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera->radius -= cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        camera->radius += cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->yaw -= cameraSpeedr;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->yaw += cameraSpeedr;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->height.y += cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->height.y -= cameraSpeedz;

    camera->radius = glm::clamp(camera->radius, 1.0f, 100.0f);
}

}