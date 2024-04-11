#include "object.hh"
#include "../utils/utils.hh"

Sphere::Sphere(Point3 center_, double radius_, Uniform_Texture uniformMaterial_) {
    center = center_;
    radius = radius_;
    uniformMaterial = uniformMaterial_;
}

Sphere::Sphere(Point3 center_, Uniform_Texture uniformMaterial_) {
    center = center_;
    radius = 1;
    uniformMaterial = uniformMaterial_;
}

double Sphere::ray_intersection(Point3 cam_center, Vector3 dir) {
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

Vector3 Sphere::normale(Point3 point)
{
    return (point - center).norm();
}

Uniform_Texture Sphere::get_material()
{
    return uniformMaterial;
}