#include "camera.hh"
#include "env.hh"

Camera::Camera() {
    position_ = glm::vec3(0.0f, 0.0f, 0.0f);
    up_ = glm::vec3(0.0f, 1.0f, 0.0f);
    height_ = glm::vec3(0.0f, 5.0f, 0.0f);
}

Camera::Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 height) {
    position_ = pos;
    up_ = up;
    height_ = height;
}

glm::vec3 Camera::position() const {
    return position_;
}
glm::vec3 Camera::up() const {
    return up_;
}

float Camera::ratioZoom() const {
    return radius_ / 200.0f;
}

void Camera::processInputs(GLFWwindow *window, float deltaTime) {
    float cameraSpeedz = speedZoom_ * deltaTime;
    float cameraSpeedr = speedRotation_ * 10.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        radius_ -= cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        radius_ += cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        yaw_ -= cameraSpeedr;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        yaw_ += cameraSpeedr;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        height_.y += cameraSpeedz;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        height_.y -= cameraSpeedz;

    radius_ = glm::clamp(radius_, 1.0f, 100.0f);

    position_.x = radius_ * cos(glm::radians(yaw_));
    position_.z = radius_ * sin(glm::radians(yaw_));
    position_.y = height_.y;
}

glm::vec3 Camera::getMouseRay(float mouseX, float mouseY,
                              float windowWidth, float windowHeight) const
{
    Env::view = glm::lookAt(position_, glm::vec3(0.0f), up_);

    // Normalized Device Coordinates
    float x = 2.0f * mouseX / windowWidth - 1.0f;
    float y = 1.0f - 2.0f * mouseY / windowHeight;
    float z = 1.0f;

    // Clip space
    glm::vec4 rayClip(x, y, -1.0f, 1.0f);

    // Eye space
    glm::vec4 rayEye = glm::inverse(Env::projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    // World space
    glm::vec4 rayWorld4 = glm::inverse(Env::view) * rayEye;
    glm::vec3 rayWorld = glm::normalize(glm::vec3(rayWorld4));

    return rayWorld;
}