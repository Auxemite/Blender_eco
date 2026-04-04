#pragma once

inline float sqr(float x) { return x * x; }
inline float discr(float a, float b, float c) { return sqr(b) - (4 * a * c); }
inline float abs_(float x) { return x < 0 ? -x : x; }

inline glm::mat3 getRotationMatrix(const glm::vec3& angle) {
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