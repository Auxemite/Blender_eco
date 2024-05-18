#include <string>
#include "object.hh"
#include "../utils/utils.hh"

int object_index = 0;

Shape_data::Shape_data(const Point3& center_, double radius_) { // Sphere
    origin = center_;
    radius = radius_;
    obj_type = "Sphere";
};
Shape_data::Shape_data(const Point3& origin_, const Vector3& normal, bool grille_) { // Plane
    origin = origin_;
    normal_ = normal;
    grille = grille_;
    obj_type = "Plane";
};
Shape_data::Shape_data(const Point3& a_, const Point3& b_, const Point3& c_, const Vector3& normal) { // Triangle
    a = a_;
    b = b_;
    c = c_;
    normal_ = normal;
    obj_type = "Triangle";
};

/////////////////// Sphere //////////////////////////////
Sphere::Sphere(const Point3& center_, double radius_, Uniform_Texture uniformMaterial_) 
{
    center = center_;
    radius = radius_;
    texture = uniformMaterial_;
    object_index++;
}

Sphere::Sphere(const Point3& center_, Uniform_Texture uniformMaterial_) 
{
    center = center_;
    radius = 1;

    texture = uniformMaterial_;
    // name = "Sphere " + to_string(object_index);
    object_index++;
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

Shape_data Sphere::get_obj_data() const
{
    return Shape_data(center, radius);
};

/////////////////// Plane /////////////////////////////////
Plane::Plane(const Point3& center_, Vector3 normal_, Uniform_Texture uniformMaterial_, bool grille_)
{
    origin = center_;
    this->normal_ = normal_.norm();
    texture = uniformMaterial_;
    grille = grille_;
}

double grille_intersection(const Point3& point_loc, double distance, const Vector3& normal)
{
    double diff_1 = abs_(round(point_loc.x) - point_loc.x);
    double diff_2 = abs_(round(point_loc.z) - point_loc.z);
    if (normal.x == 1) {
        diff_1 = abs_(round(point_loc.y) - point_loc.y);
        diff_2 = abs_(round(point_loc.z) - point_loc.z);
    }
    else if (normal.z == 1) {
        diff_1 = abs_(round(point_loc.x) - point_loc.x);
        diff_2 = abs_(round(point_loc.y) - point_loc.y);
    }

//    if (diff_1 > abs_(diff_1 - diff_2) && diff_2 > abs_(diff_1 - diff_2))
//        return -1; // For beautiful pattern
    auto diff = 0.3;
    if (diff_1 * 20 + diff_2 > diff && diff_1 + diff_2 * 20 > diff)
        return -1;

    return distance;
}

double Plane::ray_intersection(const Point3& cam_position, const Vector3& direction)
{
    double ray_dot_normal = dot(normal_, direction);

    // Parallel
    if (abs_(ray_dot_normal) < 0.01)
        return -1;

    auto distance = dot(origin - cam_position, this->normal_) / ray_dot_normal;
    if (grille) // Si le plan est de type grille
        return grille_intersection(cam_position + direction * distance, distance, this->normal_);

    // Check distance
    return distance;
}

Vector3 Plane::normal(const Point3& point) const { return this->normal_; }
Material Plane::get_material(const Point3& point) const { return texture.get_texture(point); }
Shape_data Plane::get_obj_data() const
{
    return Shape_data(origin, normal_, grille);
};

/////////////////// Triangle ///////////////////////////////
Triangle::Triangle(const Point3& a_, const Point3& b_, const Point3& c_, Uniform_Texture uniformMaterial_)
{
    a = new Point3(a_);
    b = new Point3(b_);
    c = new Point3(c_);

    normal_ = cross((*b - *a), (*c - *a));
    normal_.normalize();

    texture = uniformMaterial_;
}

Triangle::Triangle(Point3 *a_, Point3 *b_, Point3 *c_, Uniform_Texture uniformMaterial_)
{
    a = a_;
    b = b_;
    c = c_;

    normal_ = cross((*b - *a), (*c - *a));
    normal_.normalize();

    texture = uniformMaterial_;
}

double Triangle::ray_intersection(const Point3& cam_position, const Vector3& direction)
{
    Vector3 edge_1 = *b - *a;
    Vector3 edge_2 = *c - *a;
    Vector3 normal_vect = direction * edge_2;
    double det = dot(edge_1, normal_vect);

    // Parallel
    if (abs_(det) <= 0.001)
        return -1.;

    double inv_det = 1.0 / det;
    Vector3 s = cam_position - *a;
    double u = inv_det * dot(s, normal_vect);

    if (u < 0 || u > 1)
        return -1.;

    Vector3 s_cross_e1 = s * edge_1;
    double v = inv_det * dot(direction, s_cross_e1);

    if (v < 0 || u + v > 1)
        return -1.;

    double t = inv_det * dot(edge_2, s_cross_e1);

    /* if (t < 0.01)
        return -1.; */

    return t;
}

Vector3 Triangle::normal(const Point3& point) const { return this->normal_; }
Material Triangle::get_material(const Point3& point) const { return texture.get_texture(point); }
Shape_data Triangle::get_obj_data() const
{
    return Shape_data(*a, *b, *c, normal_);
};
