#include "object.hh"
#include "../utils/utils.hh"

/////////////////// Sphere //////////////////////////////
Sphere::Sphere(const Point3& center_, double radius_, Uniform_Texture uniformMaterial_) 
{
    center = center_;
    radius = radius_;
    texture = uniformMaterial_;
}

Sphere::Sphere(const Point3& center_, Uniform_Texture uniformMaterial_) 
{
    center = center_;
    radius = 1;
    texture = uniformMaterial_;
}

double Sphere::ray_intersection(const Point3& cam_center, const Vector3& dir) {
    Vector3 oc = cam_center - center;

    auto a = dot(dir, dir);
    auto b = 2.0 * dot(oc, dir);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = discr(a, b, c);

    if (discriminant < 0)
        return -1.0;
    else
        return (-b - sqrt(discriminant)) / (2.0 * a);
}

Vector3 Sphere::normal(const Point3& point) const
{
    return (point - center).norm();
}

Material Sphere::get_material(const Point3& point) const
{
    return texture.get_texture(point);
}


/////////////////// Plane /////////////////////////////////
Plane::Plane(const Point3& center_, Vector3 normal_, Uniform_Texture uniformMaterial_)
{
    origin = center_;
    this->normal_ = normal_.norm();
    texture = uniformMaterial_;
}

double Plane::ray_intersection(const Point3& cam_position, const Vector3& direction)
{
    double ray_dot_normal = dot(normal_, direction);

    // Parallel
    if (ray_dot_normal < 0.001)
        return -1;

    // Check distance
    return dot(origin - cam_position, this->normal_) / ray_dot_normal;
}

Vector3 Plane::normal(const Point3& point) const { return this->normal_; }
Material Plane::get_material(const Point3& point) const { return texture.get_texture(point); }


/////////////////// Triangle ///////////////////////////////
Triangle::Triangle(const Point3& a_, const Point3& b_, const Point3& c_, Uniform_Texture uniformMaterial_)
{
    a = a_;
    b = b_;
    c = c_;

    normal_ = cross((b - a), (c - a));
    normal_.normalize();

    texture = uniformMaterial_;
}

double Triangle::ray_intersection(const Point3& cam_position, const Vector3& direction)
{
    double det = dot((b - a), normal_);

    // Parallel
    if (abs(det) <= 0.001)
        return false;

    double inv_det = 1.0 / det;
    Vector3 s = cam_position - a;
    double u = inv_det * dot(s, normal_);

    if (u < 0 || u > 1)
        return false;

    Vector3 s_cross_e1 = cross(s, (b - a));
    float v = inv_det * dot(direction, s_cross_e1);

    if (v < 0 || u + v > 1)
        return false;

    double t = inv_det * dot((c - a), s_cross_e1);

    if (t < 0.001)
        return -1.;

    return t;
}

Vector3 Triangle::normal(const Point3& point) const { return this->normal_; }
Material Triangle::get_material(const Point3& point) const { return texture.get_texture(point); }
