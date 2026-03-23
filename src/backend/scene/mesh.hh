#pragma once

#define PI 3.14159265f
#include <vector>
#include "object.hh"

struct Edge
{
    Point3 *a;
    Point3 *b;
};

class Mesh
{
public:
    Uniform_Texture texture;
    std::vector<Point3 *> points;
    std::vector<Triangle *> faces;
    Sphere hit_box;
    bool watch=true;

    Mesh() = default;
    explicit Mesh(Mesh *mesh);
    Mesh(const Point3& a_, const Point3& b_, const Point3& c_, const Uniform_Texture& uniformMaterial_);
    Mesh(std::vector<Point3 *> points_vec, const std::vector<std::vector<int>>& faces_vec, const Uniform_Texture& uniformMaterial_);
    Mesh(const std::string& filename, const Uniform_Texture& uniformMaterial_); //From .obj

    int get_point_index(const Point3* point) const;
    int get_real_point_index(const Point3* point) const;
    std::vector<int> get_face_index(const Triangle& face) const;
    void to_dot_obj(const std::string& filename); //To .obj

    // Hit_box
    void update_hit_box();
    void update_hit_box(const Point3& point);
    Point3 get_mid(const std::vector<Point3 *>& pts);
    Point3 get_mid();

    // Displacement
    //// Point
    bool move_point(Point3 *point, const Point3& new_pos);
    bool translate_point(Point3 *point, const Point3& new_pos);
    //// Mesh
    bool move_mesh(const Point3& new_pos);
    bool translate_mesh(const Point3& new_pos);

    // Creation
    //// Point
    bool add_point(Point3 *point);
    bool create_point(const Point3& point);
    bool create_point(float a, float b, float c);
    //// Face
    bool add_face(Triangle *new_triangle);
    bool create_face(const Triangle& new_triangle);
    bool create_face(Point3 *a, Point3 *b, Point3 *c, bool add_points);
    bool move_face(Triangle *face, const Point3& new_pos);
    bool move_face(std::vector<Triangle *> face_list, const Point3& new_pos);
    void scale_face(float size, Triangle *face);
    void scale_face(float size, std::vector<Triangle *> face_list);

    // Usefull
    std::vector<Point3 *> get_points_from_indexes(const std::vector<int>&) const;

    // Dimension
    //// All Mesh
    void scale_mesh(float size);
    void scale_mesh(float size, const Point3& from);
    //// Selected Points of the Mesh
    void scale_selected(float size, const std::vector<int>& indexes);
    void scale_selected(float size, const Point3& from, const std::vector<int>& indexes);
    static void scale_selected(float size, const Point3& from, const std::vector<Point3 *>& point_list);

    // Rotation
    //// All Mesh
    void rotate_axis_x(float angle);
//    void rotate_axis_y(float angle);
//    void rotate_axis_z(float angle);
    static void rotate_point_all_axis(float angle_x, float angle_y, float angle_z, Point3 *point, const Point3& from);
    //// Selection
    void rotate_axis_x(float angle, const std::vector<Point3 *>& points);
//    void rotate_axis_y(float angle, std::vector<Point3 *> points);
//    void rotate_axis_z(float angle, std::vector<Point3 *> points);
    void rotate_all_axis(float angle_x, float angle_y, float angle_z, const std::vector<Point3 *>& points);
    void rotate_all_axis(float angle_x, float angle_y, float angle_z);

    // Extrude
    void extrude_face(Triangle *face, Point3* a, Point3 *b, Point3 *c);
    void extrude_along_normal(float thickness, Triangle *face);
    void extrude_along_normal(float thickness, const std::vector<Triangle *>& faces);

    std::vector<Triangle *> get_faces(const Point3 *point);
    Vector3* get_point_normal(const Point3 *point);
    void extrude_along_points(float thickness, Triangle *face);
    void extrude_along_points(float thickness, const std::vector<Triangle *>& faces);
    void extrude_along_points_normalized(float thickness, Triangle *face);
    void extrude_along_points_normalized(float thickness, const std::vector<Triangle *>& faces);

    static std::vector<Edge> get_edges(const std::vector<Triangle *>& faces_, std::vector<int> *count);
    static std::vector<Edge> get_border_edges(const std::vector<Triangle *>& faces_);
    static std::vector<Point3 *> get_points_from_faces(const std::vector<Triangle *>& faces_);
    std::vector<Vector3 *> get_points_normal(const std::vector<Point3 *>& point_list);
};
