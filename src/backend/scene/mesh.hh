#pragma once

#include <vector>

#include "object.hh"


class Mesh : public Shape
{
public:
    std::vector<Point3 *> points;
    std::vector<Triangle *> faces;

    Mesh() = default;
    Mesh(const Point3& a_, const Point3& b_, const Point3& c_, Uniform_Texture uniformMaterial_);
    Mesh(std::vector<Point3 *> points_vec, std::vector<std::vector<int>> faces_vec, Uniform_Texture uniformMaterial_);
    Mesh(std::string filename, Uniform_Texture uniformMaterial_); //From .obj

    double ray_intersection(const Point3& cam_position, const Vector3& direction) override;
    Vector3 normal(const Point3& point) const override;
    Material get_material(const Point3& point) const override;
    Shape_data get_obj_data() const override;

    int get_point_index(const Point3* point) const;
    std::vector<int> get_face_index(const Triangle& face) const;
    void to_dot_obj(std::string filename); //To .obj

    bool move_point(int index, const Point3& new_pos);
};
