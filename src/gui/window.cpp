#include "window.hh"
#include "imgui.h"
#include "glad/gl.h"
#include "guiUtils.hh"

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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key != GLFW_KEY_ESCAPE && ImGui::GetIO().WantCaptureKeyboard)
        return;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        std::cout  << "Toggling Fullscreen\n";
        WindowState* state = static_cast<WindowState*>(glfwGetWindowUserPointer(window));
        toggleFullscreen(window, *state);
    }
}

GLFWwindow *softwareContextInit() {
    if (!glfwInit()) {
        std::cerr << "Error glfw_window_init : Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Disable Window Decoration
//    glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Wings", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error glfw_window_init : Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, Window::framebufferSizeCallback);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Error glfw_window_init : Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    Gui::initialize(window);
    WindowState windowState;
    glfwSetWindowUserPointer(window, &windowState);
    glfwSetKeyCallback(window, Window::keyCallback);

    return window;
}

void toggleFullscreen(GLFWwindow* window, WindowState& state) {
    state.fullScreen = !state.fullScreen;

    if (state.fullScreen)
    {
        glfwGetWindowPos(window, &state.windowPosX, &state.windowPosY);
        glfwGetWindowSize(window, &state.windowWidth, &state.windowHeight);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwSetWindowMonitor(
                window,
                monitor,
                0, 0,
                mode->width,
                mode->height,
                mode->refreshRate
        );
    }
    else
    {
        glfwSetWindowMonitor(window,nullptr,
                             state.windowPosX,state.windowPosY,
                             state.windowWidth,state.windowHeight,0);
    }
}

void shutDown(GLFWwindow *window) {
    Gui::shutDown();
    glfwDestroyWindow(window);
    glfwTerminate();
}

// return 0 if normal and -1 for error
int processInput(GLFWwindow *window, Scene& scene, EditMode::EditModeScene& editModeScene) {
    Env::shiftMode = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
            || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

    float currentFrame = static_cast<float>(glfwGetTime());
    float deltaTime = currentFrame - Env::lastFrame;
    Env::lastFrame = currentFrame;
    if (Env::editmode)
        editModeScene.processInputs(window, deltaTime);
    else
        scene.processInputs(window, deltaTime);

    return 0;
}

}