#pragma once

#include "env.hh"

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "backend/camera.hh"

struct WindowState
{
    bool fullScreen = false;

    int windowPosX = 0;
    int windowPosY = 0;
    int windowWidth = 1280;
    int windowHeight = 720;
};

namespace Window {
    void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    void glfwErrorCallback(int error, const char *description);
    void toggleFullscreen(GLFWwindow* window, WindowState& state);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    GLFWwindow *glfwWindowInit();
    void shutDown(GLFWwindow *window);
    int processInput(GLFWwindow *window, Camera *camera);

}