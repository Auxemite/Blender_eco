#include "mathUtils.hh"

float Math::sqr(float x) {return x * x; }
float Math::discr(float a, float b, float c) { return Math::sqr(b) - (4 * a * c); }
float Math::abs(float x) { return x < 0 ? -x : x; }

glm::mat3 Math::getRotationMatrix(const glm::vec3& angle) {
    glm::mat3 matX = {1.0, 0.0, 0.0,
                      0.0, cos(angle.x), -sin(angle.x),
                      0.0, sin(angle.x), cos(angle.x) };
    glm::mat3 matY = {cos(angle.y), 0.0, sin(angle.y),
                      0.0, 1.0, 0.0,
                      -sin(angle.y), 0.0, cos(angle.y) };
    glm::mat3 matZ = {cos(angle.z), -sin(angle.z), 0.0,
                      sin(angle.z), cos(angle.z), 0.0,
                      0.0, 0.0, 1.0 };
    return matX * matY * matZ;
}

float Math::raySphereIntersection(const glm::vec3& cameraPos, const glm::vec3& rayDir,
                            const glm::vec3& sphereCenter, float sphereRadius) {
    glm::vec3 oc = cameraPos - sphereCenter;

    float a = dot(rayDir, rayDir);
    float b = 2.0f * dot(oc, rayDir);
    float c = dot(oc, oc) - sphereRadius * sphereRadius;
    float discriminant = Math::discr(a, b, c);

    if (discriminant < 0)
        return -1.0;
    else
        return (-b - sqrt(discriminant)) / (2.0f * a);
}

float Math::rayTriangleIntersection(const glm::vec3& cameraPos, const glm::vec3& rayDir,
                              const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3) {
    glm::vec3 edge_1 = vertex2 - vertex1;
    glm::vec3 edge_2 = vertex3 - vertex1;
    glm::vec3 normal_vect = cross(rayDir, edge_2);
    float det = dot(edge_1, normal_vect);

    // Parallel
    if (Math::abs(det) <= 0.001) {
        return false;
    }

    float inv_det = 1.0f / det;
    glm::vec3 s = cameraPos - vertex1;
    float u = inv_det * dot(s, normal_vect);

    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    glm::vec3 s_cross_e1 = cross(s, edge_1);
    float v = inv_det * dot(rayDir, s_cross_e1);

    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    return std::max(inv_det * dot(edge_2, s_cross_e1), 0.0f);
}

// TODO FIX THIS SHIT
glm::ivec2 Math::getClosestEdge(const glm::vec3& rayHitPoint,
                    const glm::vec3& vertex1, const glm::vec3& vertex2, const glm::vec3& vertex3,
                    int index1, int index2, int index3) {
    float distA = glm::distance(vertex1, rayHitPoint);
    float distB = glm::distance(vertex2, rayHitPoint);
    float distC = glm::distance(vertex3, rayHitPoint);
    float maxDist = std::max(distA, std::max(distB, distC));
    int edgeVertexIndex1 = index1;
    if (maxDist == distA)
        edgeVertexIndex1 = index3;
    int edgeVertexIndex2 = index2;
    if (maxDist == distB)
        edgeVertexIndex2 = index3;

    return {edgeVertexIndex1, edgeVertexIndex2};
}