#pragma once

#include <cmath>
#include "../utils/texture_material.hh"
#include "../utils/vector3.hh"

class Object
{
    public:
    Object() = default;

    Uniform_Texture uniformMaterial = Uniform_Texture();

    virtual double ray_intersection(Point3 p, Vector3 v) { return 0; };
    virtual Vector3 normale(Point3 point) { return {}; };
    virtual Uniform_Texture get_material() { return uniformMaterial; };
};

class Sphere : public Object {
    public:
    Point3 center = Point3();
    double radius = 0;

    Sphere()= default;
    Sphere(Point3 center_, Uniform_Texture uniformMaterial_);
    Sphere(Point3 center_, double radius_, Uniform_Texture uniformMaterial_);
//    Sphere(Point3 center_, double radius_, Uniform_Texture uniformMaterial_, const string& name_);

    double ray_intersection(Point3 cam_center, Vector3 dir) override;
    Vector3 normale(Point3 point) override;
    Uniform_Texture get_material() override;
};