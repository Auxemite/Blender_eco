#include "camera.hh"
#include "env.hh"

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

glm::vec3 Camera::getMouseRay(double mouseX, double mouseY,
                      int windowWidth, int windowHeight)
{
    Env::view = glm::lookAt(position, glm::vec3(0.0f), up);

    // Normalized Device Coordinates
    float x = (2.0f * mouseX) / windowWidth - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / windowHeight;
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