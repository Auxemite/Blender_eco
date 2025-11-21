#include "triangle.hh"
#include "utils.hh"

void Triangle::normalize()
{
    float len = std::sqrt(sqr(normal_.x) + sqr(normal_.y) + sqr(normal_.z));
    normal_ /= len;
}

Triangle::Triangle(const glm::vec3& a_, const glm::vec3& b_, const glm::vec3& c_)
{
    a = new glm::vec3(a_);
    b = new glm::vec3(b_);
    c = new glm::vec3(c_);

    selected = false;

    normal_ = cross((*b - *a), (*c - *a));
    normalize();
}

Triangle::Triangle(glm::vec3 *a_, glm::vec3 *b_, glm::vec3 *c_)
{
    a = a_;
    b = b_;
    c = c_;

    selected = false;

    normal_ = cross((*b - *a), (*c - *a));
    normalize();
}

float Triangle::ray_intersection(const glm::vec3& cam_position, const glm::vec3& direction)
{
    glm::vec3 edge_1 = *b - *a;
    glm::vec3 edge_2 = *c - *a;
    glm::vec3 normal_vect = direction * edge_2;
    float det = dot(edge_1, normal_vect);

    // Parallel
    if (abs_(det) <= 0.001)
        return -1.;

    float inv_det = 1.0f / det;
    glm::vec3 s = cam_position - *a;
    float u = inv_det * dot(s, normal_vect);

    if (u < 0 || u > 1)
        return -1.;

    glm::vec3 s_cross_e1 = s * edge_1;
    float v = inv_det * dot(direction, s_cross_e1);

    if (v < 0 || u + v > 1)
        return -1.;

    float t = inv_det * dot(edge_2, s_cross_e1);

    return t;
}

void Triangle::scale(float size) const
{
    if (size == 1.)
        return;

    glm::vec3 mid = *a / 3.0f + *b / 3.0f + *c / 3.0f;

    glm::vec3 diff_a = *a - mid;
    glm::vec3 diff_b = *b - mid;
    glm::vec3 diff_c = *c - mid;

    *a = mid + diff_a * size;
    *b = mid + diff_b * size;
    *c = mid + diff_c * size;
}

void Triangle::scale(float size, const glm::vec3& from) const
{
    if (size == 1.)
        return;

    glm::vec3 diff_a = *a - from;
    glm::vec3 diff_b = *b - from;
    glm::vec3 diff_c = *c - from;

    *a = from + diff_a * size;
    *b = from + diff_b * size;
    *c = from + diff_c * size;
}

void Triangle::update_normal()
{
    normal_ = cross((*b - *a), (*c - *a));
    normalize();
}