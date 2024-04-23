#pragma once

#include <cmath>
#include "../utils/texture_material.hh"
#include "../utils/vector3.hh"

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

class Plane : public Shape {
    public:
    Point3 origin;
    Vector3 normal_;
    bool grille;

    Plane(const Point3& center_, Vector3 normal_, Uniform_Texture uniformMaterial_, bool grille_ = false);

    double ray_intersection(const Point3& cam_position, const Vector3& direction) override;
    Vector3 normal(const Point3& point) const override;
    Material get_material(const Point3& point) const override;
};

class Triangle : public Shape
{
    public:
    Point3 a, b, c;
    Vector3 normal_;

    Triangle(const Point3& a_, const Point3& b_, const Point3& c_, Uniform_Texture uniformMaterial_);
    double ray_intersection(const Point3& cam_position, const Vector3& direction) override;
    Vector3 normal(const Point3& point) const override;
    Material get_material(const Point3& point) const override;
};