#include <iostream>
#include "triangle.hh"
#include "utils/mathUtils.hh"

void Triangle::normalize()
{
    float len = std::sqrt(Math::sqr(normal_.x) + Math::sqr(normal_.y) + Math::sqr(normal_.z));
    normal_ /= len;
}

Triangle::Triangle(glm::ivec3 vertexIndexes, glm::vec3 _normal)
{
    ia_ = vertexIndexes.x;
    ib_ = vertexIndexes.y;
    ic_ = vertexIndexes.z;
    normal_ = _normal;
    normalize();
}

Triangle::Triangle(glm::ivec3 vertexIndexes, const std::vector<glm::vec3 *>& points)
{
    ia_ = vertexIndexes.x;
    ib_ = vertexIndexes.y;
    ic_ = vertexIndexes.z;
    normal_ = cross((*points[ib_] - *points[ia_]), (*points[ic_] - *points[ia_]));
    normalize();
}

glm::ivec3 Triangle::vertexIndexes() const {
    return {ia_, ib_, ic_};
}

glm::vec3 Triangle::normal() const {
    return normal_;
}

float Triangle::rayTriangleIntersection(std::vector<glm::vec3 *> points, const glm::vec3& cameraPos, const glm::vec3& rayDir)
{
    return Math::rayTriangleIntersection(cameraPos, rayDir, *points[ia_], *points[ib_], *points[ic_]);
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