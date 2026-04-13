#pragma once

#include "glm/glm.hpp"
#include <glad/gl.h>
#include "GLFW/glfw3.h"

class Camera {
    public:
        Camera();
        Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 height);

        glm::vec3 position() const;
        glm::vec3 up() const;
        float ratioZoom() const;

        void processInputs(GLFWwindow *window, float deltaTime);
        glm::vec3 getMouseRay(float mouseX, float mouseY, float windowWidth, float windowHeight) const;

    private:
        glm::vec3 position_ = {};
        glm::vec3 up_ = {};
        glm::vec3 height_ = {}; //tmp solution

        float radius_ = 10.0f;
        float yaw_ = -45.0f;
        float pitch_ = 0.0f;
        float speedRotation_ = 30.0f;
        float speedZoom_ = 10.0f;
};