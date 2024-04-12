#include "object.hh"
#include "../utils/utils.hh"

Sphere::Sphere(const Point3& center_, double radius_, Uniform_Texture uniformMaterial_) {
    center = center_;
    radius = radius_;
    texture = uniformMaterial_;
}

Sphere::Sphere(const Point3& center_, Uniform_Texture uniformMaterial_) {
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