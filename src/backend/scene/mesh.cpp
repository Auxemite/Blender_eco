#include "mesh.hh"

#include <fstream>
#include <strstream>

Mesh::Mesh(const Point3& a_, const Point3& b_, const Point3& c_, Uniform_Texture uniformMaterial_)
{
    faces.push_back(new Triangle(a_, b_, c_, uniformMaterial_));
    points.push_back(new Point3(a_));
    points.push_back(new Point3(b_));
    points.push_back(new Point3(c_));

    texture = uniformMaterial_;
}

Mesh::Mesh(std::vector<Point3 *> points_vec, std::vector<std::vector<int>> faces_vec, Uniform_Texture uniformMaterial_)
{
    points = points_vec;

    for (auto face_point : faces_vec)
    {
        faces.push_back(new Triangle(*points_vec[face_point[0]],
                                     *points_vec[face_point[1]],
                                     *points_vec[face_point[2]],
                                     uniformMaterial_
                                    ));
    }

    texture = uniformMaterial_;
}

#define LINE_LEN 512
Mesh::Mesh(std::string filename, Uniform_Texture uniformMaterial_)
{
    // Open file as stream
    std::ifstream f(filename);
    if (!f.is_open())
        return;

    Point3 p;
    char skip;
    
    while (!f.eof())
    {
        // Foreach line
        char line[LINE_LEN];
        f.getline(line, LINE_LEN);

        std::strstream s;
        s << line;
        s >> skip;

        // if first char is 'v' create associated point
        if (skip == 'v' && s.peek() == ' ')
        {
            s >> p.x >> p.y >> p.z;
            points.push_back(new Point3(p));
        }

        // if first char is 'f' create associated face
        if (skip == 'f')
        {
            // Handle different format such as "f 3 6 9" and "f 3/4/5 6/7/8 9/10/11"
            int f[3];
            for (int i = 0; i < 3; i++)
            {
                s >> f[i];
                while (s.peek() != ' ' && !s.eof())
                    s >> skip;
            }

            // point index -1 because starting index is 1 in the .obj file
            faces.push_back(new Triangle(points[f[0] - 1],
                                         points[f[1] - 1],
                                         points[f[2] - 1],
                                         uniformMaterial_));
        }
    }

    texture = uniformMaterial_;
}

int Mesh::get_point_index(const Point3* point) const
{
    int i = 1;
    for (auto pt : points)
    {
        if (pt == point)
            return i;
        i++;
    }

    std::cerr << "Cannot find point index" << std::endl;
    return -1;
}

std::vector<int> Mesh::get_face_index(const Triangle& face) const
{
    return std::vector<int>({
        get_point_index(face.a),
        get_point_index(face.b),
        get_point_index(face.c),
    });
}

void Mesh::to_dot_obj(std::string filename)
{
    std::ofstream f(filename);
    if (!f)
        return;
    
    f << "#Generated with Blender Eco++\n";

    for (auto point : points)
        f << "v " << point->x << ' ' << point->y << ' ' << point->z << '\n';

    f << '\n';

    for (auto face : faces)
    {
        std::vector<int> index = get_face_index(*face);

        f << "f " << index[0] << ' ' << index[1] << ' ' << index[2] << '\n';
    }
}

bool Mesh::move_point(int index, const Point3& new_pos)
{
    try
    {
        Point3 *point = points.at(index);
        point->x = new_pos.x;
        point->y = new_pos.y;
        point->z = new_pos.z;
        
        std::cout << "Point moved to " << new_pos << std::endl;

        return true;
    }
    catch (std::out_of_range const& exc)
    {
        std::cout << "Point out of range\n";
        return false;
    }
}

bool Mesh::translate_point(int index, const Point3& new_pos)
{
    try
    {
        Point3 *point = points.at(index);
        *point += new_pos;
        
        std::cout << "Point translated by " << new_pos << std::endl;

        return true;
    }
    catch (std::out_of_range const& exc)
    {
        std::cout << "Point out of range\n";
        return false;
    }
}

bool Mesh::move_mesh(const Point3& new_pos)
{
    try
    {
        Point3 *origin = points.at(0);
        Vector3 diff = new_pos - *origin; 
        for (auto point : points)
            *point += diff;

            std::cout << "Mesh moved to " << new_pos << std::endl;

        return true;
    }
    catch (std::out_of_range const& exc)
    {
        std::cerr << "No origin cannot move mesh\n";
        return false;
    }
}

bool Mesh::translate_mesh(const Point3& new_pos)
{
    for (auto point : points)
        *point += new_pos;
        
        std::cout << "Mesh translated by " << new_pos << std::endl;
    
    return true;
}

bool Mesh::add_point(Point3 *new_point)
{
    for (auto point : points)
        if (point == new_point)
            return false;

    points.push_back(new_point);
    return true;
}

bool Mesh::create_point(const Point3& point)
{
    points.push_back(new Point3(point));
    return true;
}

bool Mesh::create_point(double a, double b, double c)
{
    points.push_back(new Point3(a, b, c));
    return true;
}
    
bool Mesh::add_face(Triangle *new_face)
{
    for (auto face : faces)
        if (face == new_face)
            return false;

    faces.push_back(new_face);

    add_point(new_face->a);
    add_point(new_face->b);
    add_point(new_face->c);

    return true;
}

bool Mesh::create_face(const Triangle& new_face)
{
    Triangle *face = new Triangle(new_face);
    faces.push_back(face);

    add_point(face->a);
    add_point(face->b);
    add_point(face->c);

    return true;
}

bool Mesh::create_face(Point3 *a, Point3 *b, Point3 *c)
{
    Triangle *face = new Triangle(a, b, c, texture);
    faces.push_back(face);

    add_point(face->a);
    add_point(face->b);
    add_point(face->c);

    return true;
}

// Dimension
void Mesh::scale_mesh(double size)
{
    if (size == 1.)
        return;

    Point3 mid = (0, 0, 0);
    int nb_point = points.size();
    for (auto point : points)
        mid += *point / nb_point;

    for (auto point : points)
        *point = mid + (*point - mid) * size;
}

void Mesh::scale_mesh(double size, const Point3& from)
{
    if (size == 1.)
        return;

    for (auto point : points)
        *point = from + (*point - from) * size;  
}

inline std::vector<Point3 *> Mesh::get_points_from_indexes(const std::vector<int> indexes) const
{
    std::vector<Point3 *> point_list;

    try
    {
        for (int i : indexes)
            point_list.push_back(points.at(i));
    }
    catch (std::out_of_range const& exc)
    {
        std::cerr << "Invalid index given for rescaling\n";
    }

    return point_list;
}

void Mesh::scale_selected(double size, const std::vector<int> indexes)
{
    if (size == 1.)
        return;

    std::vector<Point3 *> point_list = get_points_from_indexes(indexes);

    Point3 mid = (0, 0, 0);
    int nb_point = point_list.size();
    for (auto point : point_list)
        mid += *point / nb_point;

    for (auto point : point_list)
        *point = mid + (*point - mid) * size;
}

void Mesh::scale_selected(double size, const Point3& from, const std::vector<int> indexes)
{
    if (size == 1.)
        return;

    std::vector<Point3 *> point_list = get_points_from_indexes(indexes);

    for (auto point : point_list)
        *point = from + (*point - from) * size;
}

inline void rotate_point_axis(double angle, Point3 *point, const Point3& from)
{
    // double x = point->x - from.x;
    double y = point->y - from.y;
    double z = point->z - from.z;

    // point->x = x * std::cos(angle) - z * sin(angle);
    point->y = y * std::cos(angle) - z * sin(angle) + from.y;
    point->z = y * std::sin(angle) + z * cos(angle) + from.z;

}

void Mesh::rotate_axis(double angle)
{
    Point3 mid = (0, 0, 0);
    int nb_point = points.size();
    for (auto point : points)
        mid += *point / nb_point;

    for (auto point : points)
        rotate_point_axis(angle, point, mid);

    // Update the normals of the faces
    for (auto face : faces)
        face->update_normal();
}

void Mesh::rotate_axis(double angle, std::vector<int> indexes)
{
    std::vector<Point3 *> point_list = get_points_from_indexes(indexes);

    Point3 mid = (0, 0, 0);
    int nb_point = point_list.size();
    for (auto point : point_list)
        mid += *point / nb_point;

    for (auto point : point_list)
        rotate_point_axis(angle, point, mid);

    // Update the normals of the faces; TODO only update changed faces
    for (auto face : faces)
        face->update_normal();
}

void Mesh::extrude_face(Triangle *face, Point3* a, Point3 *b, Point3 *c)
{
    Triangle *face_1 = new Triangle(face->a, face->b, a, texture);
    Triangle *face_2 = new Triangle(face->b, b, a, texture);
    faces.push_back(face_1);
    faces.push_back(face_2);

    Triangle *face_3 = new Triangle(face->b, face->c, b, texture);
    Triangle *face_4 = new Triangle(face->c, c, b, texture);
    faces.push_back(face_3);
    faces.push_back(face_4);

    Triangle *face_5 = new Triangle(face->c, face->a, c, texture);
    Triangle *face_6 = new Triangle(face->a, a, c, texture);
    faces.push_back(face_5);
    faces.push_back(face_6);

    face->a = a;
    face->b = b;
    face->c = c;

    // Those are new points no need to check if they are already contained in the Mesh
    points.push_back(a);
    points.push_back(b);
    points.push_back(c);

    Vector3 n = cross((*b - *a), (*c - *a));
    n.normalize();
    std::cout << "Same normal ? " << (face->normal_ == n) << std::endl;
}

void Mesh::extrude_along_normal(double thickness, Triangle *face)
{
    Point3 *a = new Point3(*face->a + face->normal_ * thickness);
    Point3 *b = new Point3(*face->b + face->normal_ * thickness);
    Point3 *c = new Point3(*face->c + face->normal_ * thickness);

    extrude_face(face, a, b, c);
}

inline std::vector<Triangle *> Mesh::get_faces(const Point3 *point)
{
    std::vector<Triangle *> connected_faces;

    for (auto face : faces)
        if (face->a == point || face->b == point || face->c == point)
            connected_faces.push_back(face);
    
    return connected_faces;
}

Vector3* Mesh::get_point_normal(const Point3 *point)
{
    std::vector<Triangle *> connected = get_faces(point);
    Vector3 *normal = new Vector3(0, 0, 0);

    for (const auto face : connected)
        *normal += face->normal_;
    
    normal->normalize();
    return normal;
}

void Mesh::extrude_along_points_normalized(double thickness, Triangle *face)
{
    Point3 *a = new Point3(*face->a + *get_point_normal(face->a) * thickness);
    Point3 *b = new Point3(*face->b + *get_point_normal(face->b) * thickness);
    Point3 *c = new Point3(*face->c + *get_point_normal(face->c) * thickness);

    extrude_face(face, a, b, c);
}



void Mesh::extrude_along_points(double thickness, Triangle *face)
{
    Vector3 *normal_a = get_point_normal(face->a);
    Vector3 *normal_b = get_point_normal(face->b);
    Vector3 *normal_c = get_point_normal(face->c);

    double dot_a = dot(face->normal_, *normal_a);
    double dot_b = dot(face->normal_, *normal_b);
    double dot_c = dot(face->normal_, *normal_c);

    *normal_a /= dot_a; 
    *normal_b /= dot_b; 
    *normal_c /= dot_c; 

    Point3 *a = new Point3(*face->a + *normal_a * thickness);
    Point3 *b = new Point3(*face->b + *normal_b * thickness);
    Point3 *c = new Point3(*face->c + *normal_c * thickness);

    extrude_face(face, a, b, c);
}