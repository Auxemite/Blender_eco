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

void rotate_point_axis(double angle, Point3 *point, const Point3& from)
{
    double x = point->x - from.x;
    double y = point->y - from.y;

    point->x = x * std::cos(angle) - y * sin(angle);
    point->y = x * std::sin(angle) + y * cos(angle);
}

void Mesh::rotate_axis(double angle)
{
    Point3 mid = (0, 0, 0);
    int nb_point = points.size();
    for (auto point : points)
        mid += *point / nb_point;

    for (auto point : points)
        rotate_point_axis(angle, point, mid);
}
