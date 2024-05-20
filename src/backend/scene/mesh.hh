#pragma once

#include <vector>

#include "object.hh"


class Mesh
{
public:
    Uniform_Texture texture;
    std::vector<Point3 *> points;
    std::vector<Triangle *> faces;

    Mesh() = default;
    Mesh(const Point3& a_, const Point3& b_, const Point3& c_, Uniform_Texture uniformMaterial_);
    Mesh(std::vector<Point3 *> points_vec, std::vector<std::vector<int>> faces_vec, Uniform_Texture uniformMaterial_);
    Mesh(std::string filename, Uniform_Texture uniformMaterial_); //From .obj

    // double ray_intersection(const Point3& cam_position, const Vector3& direction) override;
    // Vector3 normal(const Point3& point) const override;
    // Material get_material(const Point3& point) const override;
    // Shape_data get_obj_data() const override;

    int get_point_index(const Point3* point) const;
    std::vector<int> get_face_index(const Triangle& face) const;
    void to_dot_obj(std::string filename); //To .obj

    // Displacement
    //// Point
    bool move_point(int index, const Point3& new_pos);
    bool translate_point(int index, const Point3& new_pos);
    //// Mesh
    bool move_mesh(int index, const Point3& new_pos);
    bool translate_mesh(int index, const Point3& new_pos);

    // Creation
    //// Point
    bool add_point(Point3 *point);
    bool create_point(const Point3& point);
    bool create_point(double a, double b, double c);
    //// Face
    bool add_face(Triangle *new_triangle);
    bool create_face(const Triangle& new_triangle);
    bool create_face(Point3 *a, Point3 *b, Point3 *c);

    // Dimension
    void scale(double size);
    void scale(double size, const Point3& from);
};
