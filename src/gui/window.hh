#pragma once

#include "env.hh"

#include <glad/gl.h>
#include "GLFW/glfw3.h"
#include "scene/scene.hh"
#include "editmode/editmodeScene.hh"

namespace Window {

    void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    void glfwErrorCallback(int error, const char *description);
    GLFWwindow *softwareContextInit();

    void toggleFullscreen(GLFWwindow* window);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void shutDown(GLFWwindow *window);
    int processInput(GLFWwindow *window, Scene& scene, EditMode::EditModeScene& editModeScene);

}