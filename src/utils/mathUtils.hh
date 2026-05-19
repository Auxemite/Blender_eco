#pragma once

#include "glm/glm.hpp"
#include "scene/triangle.hh"

namespace Math {

    struct RayInfo {
        glm::vec3 cameraPos;
        glm::vec3 rayDir;
        Triangle *hitFace;
        float hitScalar;
        float hitDistance;
    };

    float sqr(float x);
    float discr(float a, float b, float c);
    float abs(float x);

    glm::mat3 getRotationMatrix(const glm::vec3 &angle);

    float raySphereIntersection(const glm::vec3& cameraPos, const glm::vec3& rayDir,
                                const glm::vec3& sphereCenter, float sphereRadius);
    float rayTriangleIntersection(const glm::vec3& cameraPos, const glm::vec3& rayDir,
                                  const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3);
    glm::ivec2 getClosestEdge(const glm::vec3& rayHitPoint,
                              const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3,
                              int index1, int index2, int index3);
}