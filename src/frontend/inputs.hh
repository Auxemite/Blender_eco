#pragma once

#include "env.hh"

void processInput(GLFWwindow* window) {
    float cameraSpeedz = 10.0f * deltaTime;
    float cameraSpeedr = 300.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        radius -= cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        radius += cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        yaw -= cameraSpeedr;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        yaw += cameraSpeedr;

    radius = glm::clamp(radius, 1.0f, 100.0f);
}