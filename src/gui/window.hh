#pragma once

#include "env.hh"

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "backend/camera.hh"

namespace Window {

    void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    void glfwErrorCallback(int error, const char *description);

    GLFWwindow *glfwWindowInit();
    void shutDown(GLFWwindow *window);
    int processInput(GLFWwindow *window, Camera *camera);

}