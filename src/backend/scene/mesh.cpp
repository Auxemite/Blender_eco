#include "mesh.hh"

#include <fstream>
#include <strstream>
#include <unordered_map>

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

bool Mesh::create_face(Point3 *a, Point3 *b, Point3 *c, bool add_points=true)
{
    Triangle *face = new Triangle(a, b, c, texture);
    faces.push_back(face);

    if (add_points)
    {
        add_point(face->a);
        add_point(face->b);
        add_point(face->c);
    }

    return true;
}

// Usefull
Point3 get_mid(std::vector<Point3 *> points)
{
    Point3 mid = (0, 0, 0);
    int nb_point = points.size();
    for (auto point : points)
        mid += *point / nb_point;

    return mid;
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

// Dimension
void Mesh::scale_mesh(double size, const Point3& from)
{
    if (size == 1.)
        return;

    for (auto point : points)
        *point = from + (*point - from) * size;  
}

void Mesh::scale_mesh(double size)
{
    scale_mesh(size, get_mid(points));
}

void Mesh::scale_selected(double size, const std::vector<int> indexes)
{
    if (size == 1.)
        return;

    std::vector<Point3 *> point_list = get_points_from_indexes(indexes);

    Point3 mid = get_mid(point_list);

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

inline void rotate_point_axis_x(double angle, Point3 *point, const Point3& from)
{
    // double x = point->x - from.x;
    double y = point->y - from.y;
    double z = point->z - from.z;

    // point->x = x * std::cos(angle) - z * sin(angle);
    point->y = y * std::cos(angle) - z * sin(angle) + from.y;
    point->z = y * std::sin(angle) + z * cos(angle) + from.z;

}

void Mesh::rotate_axis_x(double angle, std::vector<Point3 *> point_list)
{

    Point3 mid = (0, 0, 0);
    int nb_point = point_list.size();
    for (auto point : point_list)
        mid += *point / nb_point;

    for (auto point : point_list)
        rotate_point_axis_x(angle, point, mid);

    // Update the normals of the faces; TODO only update changed faces
    for (auto face : faces)
        face->update_normal();
}

void Mesh::rotate_axis_x(double angle, std::vector<int> indexes)
{
    rotate_axis_x(angle, get_points_from_indexes(indexes));
}

void Mesh::rotate_axis_x(double angle)
{
    rotate_axis_x(angle, points);
}

/***********Extrude***********/
void Mesh::extrude_face(Triangle *face, Point3* a, Point3 *b, Point3 *c)
{
    // Create extruded faces
    create_face(face->a, face->b, a, false);
    create_face(face->b, b, a, false);

    create_face(face->b, face->c, b, false);
    create_face(face->c, c, b, false);

    create_face(face->c, face->a, c, false);
    create_face(face->a, a, c, false);

    // Update face position
    face->a = a;
    face->b = b;
    face->c = c;

    // Add the new points no need to check if they are already contained in the Mesh
    points.push_back(a);
    points.push_back(b);
    points.push_back(c);
}

void Mesh::extrude_along_normal(double thickness, Triangle *face)
{
    Point3 *a = new Point3(*face->a + face->normal_ * thickness);
    Point3 *b = new Point3(*face->b + face->normal_ * thickness);
    Point3 *c = new Point3(*face->c + face->normal_ * thickness);

    extrude_face(face, a, b, c);
}

void Mesh::extrude_along_normal(double thickness, std::vector<Triangle *> faces_)
{
    for (auto face : faces_)
        extrude_along_normal(thickness, face);
}

inline std::vector<Triangle *> Mesh::get_faces(const Point3 *point)
{
    std::vector<Triangle *> connected_faces;

    for (auto face : faces)
        if (face->a == point || face->b == point || face->c == point)
            connected_faces.push_back(face);
    
    return connected_faces;
}

void add_normal(std::vector<Vector3> *list, Vector3 normal)
{
    for (auto n : *list)
        if (n == normal)
            return;

    list->push_back(normal);
}

Vector3* Mesh::get_point_normal(const Point3 *point)
{
    std::vector<Triangle *> connected = get_faces(point);
    Vector3 *normal = new Vector3(0, 0, 0);
    std::vector<Vector3> connected_normals;

    for (const auto face : connected)
        add_normal(&connected_normals, face->normal_);

    // std::cout << connected_normals.at(0) << std::endl;

    for (const auto n : connected_normals)
        *normal += n;
    
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

bool add_point_(std::vector<Point3 *> *point_list, Point3 *point)
{
    for (const auto p : *point_list)
        if (p == point)
            return false;
    
    point_list->push_back(point);
    return true;
}

std::vector<Point3 *> Mesh::get_points_from_faces(const std::vector<Triangle *> faces_)
{
    std::vector<Point3 *> point_list;

    for (auto face : faces_)
    {
        add_point_(&point_list, face->a);
        add_point_(&point_list, face->b);
        add_point_(&point_list, face->c);
    }

    return point_list;
}

std::vector<Point3 *> get_points_from_edges(const std::vector<Edge> edges)
{
    std::vector<Point3 *> point_list;

    for (auto edge : edges)
    {
        add_point_(&point_list, edge.a);
        add_point_(&point_list, edge.b);
    }

    return point_list;
}

std::vector<Vector3 *> Mesh::get_points_normal(const std::vector<Point3 *> point_list)
{
    std::vector<Vector3 *> normals;
    
    for (const auto point : point_list)
        normals.push_back(get_point_normal(point));

    return normals;
}

void update_point(Triangle* face, Point3 *old_point, Point3 *new_point)
{
    if (face->a == old_point)
        face->a = new_point;

    if (face->b == old_point)
        face->b = new_point;

    if (face->c == old_point)
        face->c = new_point;
}

void add_edge(std::vector<Edge> *edges, std::vector<int> *count, Edge edge)
{
    int i = 0;
    for (auto e : *edges)
    {
        if ((e.a == edge.a && e.b == edge.b) ||
            (e.b == edge.a && e.a == edge.b))
        {
            count->at(i)++;
            return;
        }
        i++;
    }

    edges->push_back(edge);
    count->push_back(1);
}

std::vector<Edge> Mesh::get_edges(const std::vector<Triangle *> faces_, std::vector<int> *count)
{
    std::vector<Edge> edges;

    for (auto face : faces_)
    {
        Edge ab = {face->a, face->b};
        Edge bc = {face->b, face->c};
        Edge ca = {face->c, face->a};

        add_edge(&edges, count, ab);
        add_edge(&edges, count, bc);
        add_edge(&edges, count, ca);
    }

    return edges;
}
std::vector<Edge> Mesh::get_border_edges(const std::vector<Triangle *> faces_)
{
    std::vector<int> count;
    std::vector<Edge> edges = get_edges(faces_, &count);
    std::vector<Edge> borders;

    for (int i = 0; i < count.size(); i++)
        if (count.at(i) == 1)
            borders.push_back(edges.at(i));

    return borders;
}

int get_index(std::vector<Point3 *> list, Point3 *element)
{
    for (int i = 0; i < list.size(); i++)
        if (list.at(i) == element)
            return i;
    
    std::cerr << "Warning : Element not found\n";
    return -1;
}

void Mesh::extrude_along_points_normalized(double thickness, std::vector<Triangle *> faces_)
{
    std::vector<Point3 *> surrounded_points = get_points_from_faces(faces_);
    std::vector<Edge> border_edges = get_border_edges(faces_);
    std::vector<Point3 *> border_points = get_points_from_edges(border_edges);
    std::vector<Vector3 *> point_direction = get_points_normal(border_points);
    std::vector<Point3 *> new_border_points;

    // For all border points create the new extruded points and remove them from the surrounded points
    for (int i = 0; i < border_points.size(); i++)
    {
        new_border_points.push_back(new Point3(*border_points.at(i) + *point_direction.at(i) * thickness));

        int to_remove = get_index(surrounded_points, border_points.at(i));
        surrounded_points.erase(surrounded_points.begin() + to_remove);
    }

    // Update points in faces that contains border edge
    for (auto face : faces_)
        for (int i = 0; i < new_border_points.size(); i++)
            update_point(face, border_points.at(i), new_border_points.at(i));

    // For all border edges create the new extruded faces
    for (auto edge : border_edges)
    {
        Point3 *a_ext = new_border_points.at(get_index(border_points, edge.a));
        Point3 *b_ext = new_border_points.at(get_index(border_points, edge.b));

        create_face(edge.a, edge.b, a_ext);
        create_face(edge.b, b_ext, a_ext);
    }

    // Update remaining surrounded points (No need to create a new point neither to create extrude face)
    for (auto point : surrounded_points)
        *point += *get_point_normal(point) * thickness;

    // Update faces' normal
    for (auto face : faces_)
        face->update_normal();
}

Triangle* get_face_from_point(std::vector<Triangle *> faces, Point3* point)
{
    for (auto face : faces)
        if (face->a == point || face->b == point || face->c == point)
            return face;
        
    std::cerr << "Error: get_face_from_point : Cannot find point in given faces\n";
    return nullptr;
}

void Mesh::extrude_along_points(double thickness, std::vector<Triangle *> faces_)
{
    std::vector<Point3 *> surrounded_points = get_points_from_faces(faces_);
    std::vector<Edge> border_edges = get_border_edges(faces_);
    std::vector<Point3 *> border_points = get_points_from_edges(border_edges);
    std::vector<Vector3 *> point_direction = get_points_normal(border_points);
    std::vector<Point3 *> new_border_points;

    // For all border points create the new extruded points and remove them from the surrounded points
    for (int i = 0; i < border_points.size(); i++)
    {
        // Get a face connected to the point
        Triangle* face = get_face_from_point(faces_, border_points.at(i));

        // Calculating the difference between the face normal and the point normal
        double dot_new_point = dot(face->normal_, *point_direction.at(i));
        *point_direction.at(i) /= dot_new_point; 

        // Shift the new point along its normal depending on the dot and the thickness
        Point3 *new_point = new Point3(*border_points.at(i) + *point_direction.at(i) * thickness);

        new_border_points.push_back(new_point);

        int to_remove = get_index(surrounded_points, border_points.at(i));
        surrounded_points.erase(surrounded_points.begin() + to_remove);
    }

    // Update points in faces that contains border edge
    for (auto face : faces_)
        for (int i = 0; i < new_border_points.size(); i++)
            update_point(face, border_points.at(i), new_border_points.at(i));

    // For all border edges create the new extruded faces
    for (auto edge : border_edges)
    {
        Point3 *a_ext = new_border_points.at(get_index(border_points, edge.a));
        Point3 *b_ext = new_border_points.at(get_index(border_points, edge.b));

        create_face(edge.a, edge.b, a_ext);
        create_face(edge.b, b_ext, a_ext);
    }

    // Update remaining surrounded points (No need to create a new point neither to create extrude face)
    for (auto point : surrounded_points)
    {
        // Get a face connected to the point
        Triangle* face = get_face_from_point(faces_, point);
        Vector3 *point_normal = get_point_normal(point);

        // Calculating the difference between the face normal and the point normal
        double dot_new_point = dot(face->normal_, *point_normal);
        *point_normal /= dot_new_point; 

        // Update the point position by shifting it along its normal depending on the dot and the thickness
        *point += *point_normal * thickness;
    }

    // Update faces' normal
    for (auto face : faces_)
        face->update_normal();
}
