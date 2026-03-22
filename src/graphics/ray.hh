#pragma once

#include "glad/gl.h"
#include "backend/camera.hh"
#include "backend/scene.hh"

//#define RAY_VISIBLE

class Ray {
private:
    GLuint rayVAO = 0;
    GLuint rayVBO = 0;
    size_t raySize = 0;
    glm::vec3 ray = {};

public:
    explicit Ray(glm::vec3 cameraPos);

    void rayCasting(Scene *scene, float width, float height);
    void hitMeshTest(Scene *scene);
    void hitMeshFaceTest(Scene *scene);
    void hitMeshEdgeTest(Scene *scene);
    void hitMeshVertexTest(Scene *scene, float radius);

    float sphereIntersection(glm::vec3 cameraPos, glm::vec3 center, float radius);

    void draw(unsigned int shaderProgram, Camera *camera);
    std::vector<float> generateRay(glm::vec3 rayDirection, glm::vec3 cameraPos);
};

