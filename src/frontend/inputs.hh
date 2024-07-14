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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
        } else if (action == GLFW_RELEASE) {
            mousePressed = false;
            firstMouse = true;
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        float yawRad = glm::radians(yaw);
        float pitchRad = glm::radians(pitch);

        cameraFront.x = cos(yawRad) * cos(pitchRad);
        cameraFront.y = sin(pitchRad);
        cameraFront.z = sin(yawRad) * cos(pitchRad);

        cameraFront = glm::normalize(cameraFront);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    radius -= yoffset * sensitivity;
    if (radius < 1.0f)
        radius = 1.0f;
    if (radius > 10.0f)
        radius = 10.0f;
}