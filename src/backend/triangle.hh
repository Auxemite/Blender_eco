#pragma once

#include <vector>
#include "glm/glm.hpp"

class Triangle
{
public:
    int ia, ib, ic;
    glm::vec3 normal_;
    bool selected;

    Triangle(const int& ia_, const int& ib_, const int& ic_);
    bool ray_intersection(std::vector<glm::vec3 *> points, const glm::vec3& cam_position, const glm::vec3& direction);
//    glm::vec3 normal(const glm::vec3& point);
    void normalize();

    // Usefull for mesh transformation
//    void scale(float size) const;
//    void scale(float size, const glm::vec3& from) const;
//
//    void update_normal();
};