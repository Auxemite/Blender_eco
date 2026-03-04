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
    glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Wings", nullptr, nullptr);
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

void shutDown(GLFWwindow *window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// return 0 if normal, 1 if ask for shutdown and -1 for error
int processInput(GLFWwindow *window, Camera *camera) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        return 1;

    float currentFrame = static_cast<float>(glfwGetTime());
    float deltaTime = currentFrame - Env::lastFrame;
    Env::lastFrame = currentFrame;
    float cameraSpeedz = camera->speed_zoom * deltaTime;
    float cameraSpeedr = camera->speed_rotation * 10.0f * deltaTime;
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
    return 0;
}

}