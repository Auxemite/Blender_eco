#pragma once

#include "glad/gl.h"
#include "scene/camera.hh"
#include "scene/scene.hh"
#include "editmode/editmodeScene.hh"

//#define RAY_VISIBLE

class Ray {
    private:
        GLuint rayVAO = 0;
        GLuint rayVBO = 0;
        size_t raySize = 0;
        glm::vec3 ray = {};

    public:
        explicit Ray(glm::vec3 cameraPos);

        void rayCasting(Scene *scene, EditModeScene *editModeScene, float width, float height);
        void hitMeshTest(Scene *scene);
        void hitMeshFaceTest(EditModeScene *scene);
        void hitMeshEdgeTest(EditModeScene *scene);
        void hitMeshVertexTest(EditModeScene *scene, float radius);

        float sphereIntersection(glm::vec3 cameraPos, glm::vec3 center, float radius);

        void draw(unsigned int shaderProgram, Camera *camera);
        std::vector<float> generateRay(glm::vec3 rayDirection, glm::vec3 cameraPos);
};

