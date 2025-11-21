#include "camera.hh"

Camera::Camera() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    height = glm::vec3(0.0f, 5.0f, 0.0f);
}

Camera::Camera(glm::vec3 pos_, glm::vec3 up_, glm::vec3 height_) {
    position = pos_;
    up = up_;
    height = height_;
}