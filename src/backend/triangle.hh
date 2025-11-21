#pragma once

#include "glm/glm.hpp"

class Triangle
{
public:
    glm::vec3 *a, *b, *c;
    glm::vec3 normal_;
    bool selected;

    Triangle(const glm::vec3& a_, const glm::vec3& b_, const glm::vec3& c_);
    Triangle(glm::vec3 *a_, glm::vec3 *b_, glm::vec3 *c_);
    float ray_intersection(const glm::vec3& cam_position, const glm::vec3& direction);
    glm::vec3 normal(const glm::vec3& point);
    void normalize();

    // Usefull for mesh transformation
    void scale(float size) const;
    void scale(float size, const glm::vec3& from) const;

    void update_normal();
};