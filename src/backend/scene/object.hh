#pragma once

#include <cmath>
#include "../utils/texture_material.hh"
#include "../utils/vector3.hh"

class Shape_data {
    public:
    Point3 origin;
    double radius{};
    Vector3 normal_;
    bool grille{};
    Point3 a, b, c;
    std::string obj_type;
    Shape_data()=default;
    Shape_data(const Point3& center_, double radius_);
    Shape_data(const Point3& origin_, const Vector3& normal, bool grille_);
    Shape_data(const Point3& a_, const Point3& b_, const Point3& c_, const Vector3& normal);
};

class Shape
{
    public:
    Uniform_Texture texture;

    virtual double ray_intersection(const Point3& p, const Vector3& v) = 0;
    virtual Vector3 normal(const Point3& point) const = 0;
    virtual Material get_material(const Point3& point) const = 0;
    virtual Shape_data get_obj_data() const = 0;
};

class Sphere : public Shape {
    public:
    Point3 center;
    double radius = 0;

    Sphere()= default;
    Sphere(const Point3& center_, double radius_, Uniform_Texture uniformMaterial_);
    Sphere(const Point3& center_, Uniform_Texture uniformMaterial_);

    double ray_intersection(const Point3& cam_center, const Vector3& dir) override;
    Vector3 normal(const Point3& point) const override;
    Material get_material(const Point3& point) const override;
    Shape_data get_obj_data() const override;
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
    Shape_data get_obj_data() const override;
};

class Triangle : public Shape
{
    public:
    Point3 *a, *b, *c;
    Vector3 normal_;

    Triangle(const Point3& a_, const Point3& b_, const Point3& c_, Uniform_Texture uniformMaterial_);
    Triangle(Point3 *a_, Point3 *b_, Point3 *c_, Uniform_Texture uniformMaterial_);
    double ray_intersection(const Point3& cam_position, const Vector3& direction) override;
    Vector3 normal(const Point3& point) const override;
    Material get_material(const Point3& point) const override;
    Shape_data get_obj_data() const override;

    // Usefull for mesh transformation
    void scale(double size);
    void scale(double size, const Point3& from);

    void update_normal();
};