#pragma once

#include "scene/camera.hh"
#include "scene/scene.hh"
#include "editmode/editmodeScene.hh"

//#define RAY_VISIBLE

class Ray {
    public:
        explicit Ray(glm::vec3 cameraPos);

        void rayCasting(Scene& scene, float width, float height);

        void draw(unsigned int shaderProgram, const Camera& camera);
        std::vector<float> generateRay(glm::vec3 rayDirection, glm::vec3 cameraPos);

    private:
        GLuint rayVAO_ = 0;
        GLuint rayVBO_ = 0;
        size_t raySize_ = 0;
};

