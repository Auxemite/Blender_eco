#pragma once

#include "render_utils.h"

float deltaTime = 0.0f;
float radius = 3.0f;
float yaw = -90.0f;

void processInput(GLFWwindow* window) {
    float cameraSpeedz = 10.0f * deltaTime;
    float cameraSpeedr = 100.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        radius -= cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        radius += cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        yaw -= cameraSpeedr;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        yaw += cameraSpeedr;

    radius = glm::clamp(radius, 1.0f, 10.0f);
}