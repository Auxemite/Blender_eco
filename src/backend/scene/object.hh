#pragma once

#include <cmath>
#include <vector>
#include "../utils/texture_material.hh"
#include "../utils/vector3.hh"

enum Shape_type
{
    SPHERE,
    PLANE,
    TRIANGLE,
    NONE
};

class Shape
{
    public:
    Uniform_Texture texture;
    bool selected = false;

    virtual float ray_intersection(const Point3& p, const Vector3& v) = 0;
    virtual Vector3 normal(const Point3& point) const = 0;
    virtual Material get_material(const Point3& point) const = 0;
    virtual Shape_type get_obj_type() const = 0;
};

class Sphere : public Shape {
    public:
    Point3 center;
    float radius = 0;

    Sphere()= default;
    Sphere(const Point3& center_, float radius_, Uniform_Texture uniformMaterial_);
    Sphere(const Point3& center_, Uniform_Texture uniformMaterial_);

    float ray_intersection(const Point3& cam_center, const Vector3& dir) override;
    Vector3 normal(const Point3& point) const override;
    Material get_material(const Point3& point) const override;
    Shape_type get_obj_type() const override;
};

class Plane : public Shape {
    public:
    Point3 origin;
    Vector3 normal_;
    bool grille;

    Plane(const Point3& center_, const Vector3& normal_, Uniform_Texture uniformMaterial_, bool grille_ = false);

    float ray_intersection(const Point3& cam_position, const Vector3& direction) override;
    Vector3 normal(const Point3& point) const override;
    Material get_material(const Point3& point) const override;
    Shape_type get_obj_type() const override;
};

class Triangle : public Shape
{
    public:
    Point3 *a, *b, *c;
    Vector3 normal_;

    Triangle(const Point3& a_, const Point3& b_, const Point3& c_, Uniform_Texture uniformMaterial_);
    Triangle(Point3 *a_, Point3 *b_, Point3 *c_, Uniform_Texture uniformMaterial_);
    float ray_intersection(const Point3& cam_position, const Vector3& direction) override;
    Vector3 normal(const Point3& point) const override;
    Material get_material(const Point3& point) const override;
    Shape_type get_obj_type() const override;

    // Usefull for mesh transformation
    void scale(float size) const;
    void scale(float size, const Point3& from) const;

    void update_normal();
};