#pragma once

#include "env.hh"

void processInput(GLFWwindow* window) {
    float cameraSpeedz = 10.0f * deltaTime;
    float cameraSpeedr = 300.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        radius -= cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        radius += cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        yaw -= cameraSpeedr;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        yaw += cameraSpeedr;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraDec.y += cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraDec.y -= cameraSpeedz;
//    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
//        cameraDec -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeedz;
//    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
//        cameraDec += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeedz;

    radius = glm::clamp(radius, 1.0f, 100.0f);
}