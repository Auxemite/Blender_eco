#pragma once

#include "glm/glm.hpp"

class Camera {
    public:
        float radius = 10.0f;
        float yaw = -45.0f;
        float pitch = 0.0f;
        float speed_rotation = 30.0f;
        float speed_zoom = 10.0f;

        glm::vec3 position{};
        glm::vec3 up{};
        glm::vec3 height{}; //tmp solution

        Camera();
        Camera(glm::vec3 pos_, glm::vec3 up_, glm::vec3 height_);
        glm::vec3 getMouseRay(float mouseX, float mouseY, float windowWidth, float windowHeight);
};