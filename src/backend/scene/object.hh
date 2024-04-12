#pragma once

#include <cmath>
#include "../utils/texture_material.hh"
#include "../utils/vector3.hh"

/* class Object
{
    public:
    Object() = default;

    Uniform_Texture uniformMaterial = Uniform_Texture();

    virtual double ray_intersection(Point3 p, Vector3 v) { return 0; };
    virtual Vector3 normale(Point3 point) { return {}; };
    virtual Uniform_Texture get_material() { return uniformMaterial; };
}; */

class Shape
{
    public:
    Uniform_Texture texture;

    virtual double ray_intersection(const Point3& p, const Vector3& v) = 0;
    virtual Vector3 normal(const Point3& point) const = 0;
    virtual Material get_material(const Point3& point) const = 0;
};

class Sphere : public Shape {
    public:
    Point3 center = Point3();
    double radius = 0;

    Sphere()= default;
    Sphere(const Point3& center_, double radius_, Uniform_Texture uniformMaterial_);
    Sphere(const Point3& center_, Uniform_Texture uniformMaterial_);

    double ray_intersection(const Point3& cam_center, const Vector3& dir) override;
    Vector3 normal(const Point3& point) const override;
    Material get_material(const Point3& point) const override;
};