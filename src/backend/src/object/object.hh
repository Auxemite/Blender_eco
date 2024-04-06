 //
// Created by ernes on 15/03/2024.
//

#ifndef RAYTRACER_OBJECT_HH
#define RAYTRACER_OBJECT_HH

#include <utility>
#include <cmath>
#include "../texture_material.hh"
#include "../vector3.hh"

class Object
{
    public:
    Object() = default;

    Uniform_Texture uniformMaterial = Uniform_Texture();

    virtual double ray_intersection(Point3 p, Vector3 v)
    {
        return false;
    };
    virtual Vector3 normale(Point3 point)
    {
        return {};
    };
    virtual Uniform_Texture get_material()
    {
        return uniformMaterial;
    };
};

class Sphere : public Object {
    public:
    Point3 center = Point3();
    double radius = 0;

    Sphere()= default;
    Sphere(Point3 center_, double radius_, Uniform_Texture uniformMaterial_) {
        center = center_;
        radius = radius_;
        uniformMaterial = uniformMaterial_;
    };
    Sphere(Point3 center_, Uniform_Texture uniformMaterial_) {
        center = center_;
        radius = 1;
        uniformMaterial = uniformMaterial_;
    };

    double ray_intersection(Point3 cam_center, Vector3 dir) override {
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

    Vector3 normale(Point3 point) override
    {
        Vector3 v = point - center;
        return unit_vector(v);
    };

    Uniform_Texture get_material() override
    {
        return uniformMaterial;
    }
};

#endif //RAYTRACER_OBJECT_HH
