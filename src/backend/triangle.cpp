#include <iostream>
#include "triangle.hh"
#include "utils.hh"

void Triangle::normalize()
{
    float len = std::sqrt(sqr(normal_.x) + sqr(normal_.y) + sqr(normal_.z));
    normal_ /= len;
}

Triangle::Triangle(const int& ia_, const int& ib_, const int& ic_)
{
    ia = ia_;
    ib = ib_;
    ic = ic_;

    selected = false;

    normal_ = glm::vec3();
    normalize();
}

bool Triangle::ray_intersection(std::vector<glm::vec3 *> points, const glm::vec3& cam_position, const glm::vec3& direction)
{
    glm::vec3 edge_1 = *points[ib] - *points[ia];
    glm::vec3 edge_2 = *points[ic] - *points[ia];
    glm::vec3 normal_vect = cross(direction, edge_2);
    float det = dot(edge_1, normal_vect);

    // Parallel
    if (abs_(det) <= 0.001) {
        return false;
    }

    float inv_det = 1.0f / det;
    glm::vec3 s = cam_position - *points[ia];
    float u = inv_det * dot(s, normal_vect);

    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    glm::vec3 s_cross_e1 = cross(s, edge_1);
    float v = inv_det * dot(direction, s_cross_e1);

    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    float t = inv_det * dot(edge_2, s_cross_e1);

    return t > 0;
}

//void Triangle::scale(float size) const
//{
//    if (size == 1.)
//        return;
//
//    glm::vec3 mid = *a / 3.0f + *b / 3.0f + *c / 3.0f;
//
//    glm::vec3 diff_a = *a - mid;
//    glm::vec3 diff_b = *b - mid;
//    glm::vec3 diff_c = *c - mid;
//
//    *a = mid + diff_a * size;
//    *b = mid + diff_b * size;
//    *c = mid + diff_c * size;
//}
//
//void Triangle::scale(float size, const glm::vec3& from) const
//{
//    if (size == 1.)
//        return;
//
//    glm::vec3 diff_a = *a - from;
//    glm::vec3 diff_b = *b - from;
//    glm::vec3 diff_c = *c - from;
//
//    *a = from + diff_a * size;
//    *b = from + diff_b * size;
//    *c = from + diff_c * size;
//}
//
//void Triangle::update_normal()
//{
//    normal_ = cross((*b - *a), (*c - *a));
//    normalize();
//}