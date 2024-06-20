#include <string>
#include <utility>
#include "object.hh"

int object_index = 0;

/////////////////// Sphere //////////////////////////////
Sphere::Sphere(const Point3& center_, float radius_, Uniform_Texture uniformMaterial_)
{
    center = center_;
    radius = radius_;
    texture = std::move(uniformMaterial_);
    object_index++;
}

Sphere::Sphere(const Point3& center_, Uniform_Texture uniformMaterial_) 
{
    center = center_;
    radius = 1;
    texture = std::move(uniformMaterial_);
    object_index++;
}

float Sphere::ray_intersection(const Point3& cam_center, const Vector3& dir) {
    Vector3 oc = cam_center - center;

    float a = dot(dir, dir);
    float b = 2.0f * dot(oc, dir);
    float c = dot(oc, oc) - radius*radius;
    float discriminant = discr(a, b, c);

    if (discriminant < 0)
        return -1.0;
    else
        return (-b - sqrt(discriminant)) / (2.0f * a);
}

Vector3 Sphere::normal(const Point3& point) const
{
    return (point - center).norm();
}

Material Sphere::get_material(const Point3& point) const
{
    return texture.get_texture(point);
}

Shape_type Sphere::get_obj_type() const
{
    return Shape_type::SPHERE;
}

/////////////////// Plane /////////////////////////////////
Plane::Plane(const Point3& center_, const Vector3& normal_, Uniform_Texture uniformMaterial_, bool grille_)
{
    origin = center_;
    this->normal_ = normal_.norm();
    texture = std::move(uniformMaterial_);
    grille = grille_;
}

float grille_intersection(const Point3& point_loc, float distance, const Vector3& normal)
{
    float diff_1 = abs_(round(point_loc.x) - point_loc.x);
    float diff_2 = abs_(round(point_loc.z) - point_loc.z);
    if (normal.x == 1) {
        diff_1 = abs_(round(point_loc.y) - point_loc.y);
        diff_2 = abs_(round(point_loc.z) - point_loc.z);
    }
    else if (normal.z == 1) {
        diff_1 = abs_(round(point_loc.x) - point_loc.x);
        diff_2 = abs_(round(point_loc.y) - point_loc.y);
    }

//    if (diff_1 > abs_(diff_1 - diff_2) && diff_2 > abs_(diff_1 - diff_2))
//        return -1; // For beautiful pattern

//    auto diff = 0.3;
//    if (diff_1 * 20 + diff_2 > diff && diff_1 + diff_2 * 20 > diff)
//        return -1;

    if (diff_1 <= 0.015 || diff_2 <= 0.015)
        return distance;

//    if (diff_1 <= 0.03 && round(point_loc.x) == 0 && round(point_loc.z) != 0)
//        return distance;
//
//    if (diff_2 <= 0.03 && round(point_loc.z) == 0 && round(point_loc.x) != 0)
//        return distance;

    return -1;
}

float Plane::ray_intersection(const Point3& cam_position, const Vector3& direction)
{
    float ray_dot_normal = dot(normal_, direction);

    // Parallel
    if (abs_(ray_dot_normal) < 0.01)
        return -1;

    auto distance = dot(origin - cam_position, this->normal_) / ray_dot_normal;
    if (grille) // Si le plan est de type grille
        return grille_intersection(cam_position + direction * distance, distance, this->normal_);

    // Check distance
    return distance;
}

Vector3 Plane::normal(const Point3& point) const { return this->normal_; }
Material Plane::get_material(const Point3& point) const { return texture.get_texture(point); }
Shape_type Plane::get_obj_type() const
{
    return Shape_type::PLANE;
}

/////////////////// Triangle ///////////////////////////////
Triangle::Triangle(const Point3& a_, const Point3& b_, const Point3& c_, Uniform_Texture uniformMaterial_)
{
    a = new Point3(a_);
    b = new Point3(b_);
    c = new Point3(c_);

    normal_ = cross((*b - *a), (*c - *a));
    normal_.normalize();

    texture = std::move(uniformMaterial_);
}

Triangle::Triangle(Point3 *a_, Point3 *b_, Point3 *c_, Uniform_Texture uniformMaterial_)
{
    a = a_;
    b = b_;
    c = c_;

    normal_ = cross((*b - *a), (*c - *a));
    normal_.normalize();

    texture = std::move(uniformMaterial_);
}

float Triangle::ray_intersection(const Point3& cam_position, const Vector3& direction)
{
    Vector3 edge_1 = *b - *a;
    Vector3 edge_2 = *c - *a;
    Vector3 normal_vect = direction * edge_2;
    float det = dot(edge_1, normal_vect);

    // Parallel
    if (abs_(det) <= 0.001)
        return -1.;

    float inv_det = 1.0f / det;
    Vector3 s = cam_position - *a;
    float u = inv_det * dot(s, normal_vect);

    if (u < 0 || u > 1)
        return -1.;

    Vector3 s_cross_e1 = s * edge_1;
    float v = inv_det * dot(direction, s_cross_e1);

    if (v < 0 || u + v > 1)
        return -1.;

    float t = inv_det * dot(edge_2, s_cross_e1);

    /* if (t < 0.01)
        return -1.; */

    return t;
}

Vector3 Triangle::normal(const Point3& point) const { return this->normal_; }
Material Triangle::get_material(const Point3& point) const { return texture.get_texture(point); }
Shape_type Triangle::get_obj_type() const
{
    return Shape_type::TRIANGLE;
}

void Triangle::scale(float size) const
{
    if (size == 1.)
        return;

    Point3 mid = *a/3 + *b/3 + *c/3;

    Vector3 diff_a = *a - mid;   
    Vector3 diff_b = *b - mid;   
    Vector3 diff_c = *c - mid;

    *a = mid + diff_a * size;   
    *b = mid + diff_b * size;   
    *c = mid + diff_c * size;   
}

void Triangle::scale(float size, const Point3& from) const
{
    if (size == 1.)
        return;

    Vector3 diff_a = *a - from;
    Vector3 diff_b = *b - from;   
    Vector3 diff_c = *c - from;

    *a = from + diff_a * size;   
    *b = from + diff_b * size;   
    *c = from + diff_c * size;   
}

void Triangle::update_normal()
{
    normal_ = cross((*b - *a), (*c - *a));
    normal_.normalize();
}