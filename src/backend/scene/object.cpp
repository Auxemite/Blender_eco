#include <string>
#include "object.hh"

int object_index = 0;

//Sphere::Sphere(Point3 center_, double radius_, Uniform_Texture uniformMaterial_, const string& name_) {
//    center = center_;
//    radius = radius_;
//    uniformMaterial = uniformMaterial_;
//    name = name_;
//    object_index++;
//}

Sphere::Sphere(Point3 center_, double radius_, Uniform_Texture uniformMaterial_) {
    center = center_;
    radius = radius_;
    uniformMaterial = uniformMaterial_;
//    name = "Sphere " + to_string(object_index);
    object_index++;
}

Sphere::Sphere(Point3 center_, Uniform_Texture uniformMaterial_) {
    center = center_;
    radius = 1;
    uniformMaterial = uniformMaterial_;
//    name = "Sphere " + to_string(object_index);
    object_index++;
}

double Sphere::ray_intersection(Point3 cam_center, Vector3 dir) {
    Vector3 oc = cam_center - center;

    auto a = dot(dir, dir);
    auto b = 2.0 * dot(oc, dir);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;

    if (discriminant < 0)
        return -1.0;
    else
        return (-b - sqrt(discriminant)) / (2.0 * a);
}

Vector3 Sphere::normale(Point3 point)
{
    Vector3 v = point - center;
    return unit_vector(v);
}

Uniform_Texture Sphere::get_material()
{
    return uniformMaterial;
}