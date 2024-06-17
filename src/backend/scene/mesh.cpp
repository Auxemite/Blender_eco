#include "mesh.hh"

#include <fstream>
#include <strstream>
#include <unordered_map>
#include <filesystem>
namespace fs = std::filesystem;

Mesh::Mesh(const Point3& a_, const Point3& b_, const Point3& c_, const Uniform_Texture& uniformMaterial_)
{
    faces.push_back(new Triangle(a_, b_, c_, uniformMaterial_));
    points.push_back(new Point3(a_));
    points.push_back(new Point3(b_));
    points.push_back(new Point3(c_));

    texture = uniformMaterial_;
}

Mesh::Mesh(std::vector<Point3 *> points_vec, const std::vector<std::vector<int>>& faces_vec, const Uniform_Texture& uniformMaterial_)
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
Mesh::Mesh(const std::string& filename, const Uniform_Texture& uniformMaterial_)
{
    // Open file as stream
    auto path = fs::current_path();
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cout << "Error File " << filename << " could not be opened";
        return;
    }

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
            for (unsigned int i = 0; i < 3; i++)
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
    update_hit_box();
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

void Mesh::to_dot_obj(const std::string& filename)
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

    std::cout << "File Saved\n";
}

// Hit_box
void Mesh::update_hit_box()
{
    Point3 mid = get_mid();

    hit_box.center = mid;

    float dist_max = 0.;

    for (auto point : points)
    {
        float dist = (*point - mid).length();

        if (dist > dist_max)
            dist_max = dist;
    }

    hit_box.radius = dist_max;
}

void Mesh::update_hit_box(const Point3& point)
{
    float dist_hit_box = (hit_box.center - point).length();
    if (dist_hit_box > hit_box.radius)
        hit_box.radius = dist_hit_box;
}

// Displacement
bool Mesh::move_point(int index, const Point3& new_pos)
{
    try
    {
        Point3 *point = points.at(index);
        point->x = new_pos.x;
        point->y = new_pos.y;
        point->z = new_pos.z;
        
        std::cout << "Point moved to " << new_pos << std::endl;
        update_hit_box();

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
        update_hit_box(*point);

        return true;
    }
    catch (std::out_of_range const& exc)
    {
        std::cout << "Point out of range\n";
        return false;
    }
}

bool Mesh::move_face(Triangle *face, const Point3& new_pos)
{
    try
    {
        *face->a += new_pos;
        *face->b += new_pos;
        *face->c += new_pos;
        update_hit_box(*face->a);
        update_hit_box(*face->b);
        update_hit_box(*face->c);
        std::cout << "Face translated by " << new_pos << std::endl;

        return true;
    }
    catch (std::out_of_range const& exc)
    {
        std::cout << "Point out of range\n";
        return false;
    }
}

bool Mesh::move_face(std::vector<Triangle *> face_list, const Point3& new_pos)
{
    try
    {
        for (auto face : face_list) {
            *face->a += new_pos;
            *face->b += new_pos;
            *face->c += new_pos;
            update_hit_box(*face->a);
            update_hit_box(*face->b);
            update_hit_box(*face->c);
            std::cout << "Face translated by " << new_pos << std::endl;
        }

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
        hit_box.center += diff;

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
    hit_box.center += new_pos;
        
        std::cout << "Mesh translated by " << new_pos << std::endl;
    
    return true;
}

bool Mesh::add_point(Point3 *new_point)
{
    for (auto point : points)
        if (point == new_point)
            return false;

    update_hit_box(*new_point);
    points.push_back(new_point);
    return true;
}

bool Mesh::create_point(const Point3& point)
{
    points.push_back(new Point3(point));

    update_hit_box(point);
    return true;
}

bool Mesh::create_point(float a, float b, float c)
{
    auto *new_point = new Point3(a, b, c);
    points.push_back(new_point);

    update_hit_box(*new_point);
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
    auto *face = new Triangle(new_face);
    faces.push_back(face);

    add_point(face->a);
    add_point(face->b);
    add_point(face->c);

    return true;
}

bool Mesh::create_face(Point3 *a, Point3 *b, Point3 *c, bool add_points=true)
{
    auto *face = new Triangle(a, b, c, texture);
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
Point3 Mesh::get_mid(const std::vector<Point3 *>& pts)
{
    Point3 mid = (0.0f, 0.0f, 0.0f);
    int nb_point = pts.size();
    for (auto point : pts)
        mid += *point / static_cast<float>(nb_point);

    return mid;
}

Point3 Mesh::get_mid()
{
    return get_mid(points);
}

inline std::vector<Point3 *> Mesh::get_points_from_indexes(const std::vector<int>& indexes) const
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
void Mesh::scale_selected(float size, const Point3& from, const std::vector<Point3 *>& point_list)
{
    if (size == 1.)
        return;

    for (auto point : point_list)
        *point = from + (*point - from) * size;
}

void Mesh::scale_mesh(float size, const Point3& from)
{
    scale_selected(size, from, points);
}

void Mesh::scale_face(float size, Triangle *face)
{
    std::vector<Point3 *> point_list;
    point_list.push_back(face->a);
    point_list.push_back(face->b);
    point_list.push_back(face->c);
    scale_selected(size, get_mid(point_list), point_list);
    update_hit_box();
}

void Mesh::scale_face(float size, std::vector<Triangle *> face_list)
{
    for (auto face : face_list) {
        std::vector<Point3 *> point_list;
        point_list.push_back(face->a);
        point_list.push_back(face->b);
        point_list.push_back(face->c);
        scale_selected(size, get_mid(point_list), point_list);
        update_hit_box();
    }
}

void Mesh::scale_mesh(float size)
{
    scale_selected(size, get_mid(points), points);
    update_hit_box();
}

void Mesh::scale_selected(float size, const std::vector<int>& indexes)
{
    if (size == 1.)
        return;

    std::vector<Point3 *> point_list = get_points_from_indexes(indexes);

    scale_selected(size, get_mid(point_list), point_list);
}

void Mesh::scale_selected(float size, const Point3& from, const std::vector<int>& indexes)
{
    scale_selected(size, from, get_points_from_indexes(indexes));
}

inline void rotate_point_axis_x(float angle, Point3 *point, const Point3& from)
{
    // float x = point->x - from.x;
    float y = point->y - from.y;
    float z = point->z - from.z;

    // point->x = x * std::cos(angle) - z * sin(angle);
    point->y = y * std::cos(angle) - z * sin(angle) + from.y;
    point->z = y * std::sin(angle) + z * cos(angle) + from.z;

}

void Mesh::rotate_axis_x(float angle, const std::vector<Point3 *>& point_list)
{

    Point3 mid = get_mid(point_list);

    for (auto point : point_list)
        rotate_point_axis_x(angle, point, mid);

    for (auto face : faces)
        face->update_normal();
}

void Mesh::rotate_axis_x(float angle)
{
    rotate_axis_x(angle, points);
}

void Mesh::rotate_point_all_axis(float angle_x, float angle_y, float angle_z, Point3 *point, const Point3& from)
{
    float x = point->x - from.x;
    float y = point->y - from.y;
    float z = point->z - from.z;

    float cos_x = std::cos(angle_x), sin_x = std::sin(angle_x);
    float cos_y = std::cos(angle_y), sin_y = std::sin(angle_y);
    float cos_z = std::cos(angle_z), sin_z = std::sin(angle_z);

    point->x = x * (cos_y * cos_z) + y * (sin_x * sin_y * cos_z - cos_x * sin_z) + z * (cos_x * sin_y * cos_z + sin_x * sin_z) + from.x;
    point->y = x * (cos_y * sin_z) + y * (sin_x * sin_y * sin_z + cos_x * cos_z) + z * (cos_x * sin_y * sin_z - sin_x * cos_z) + from.y;
    point->z = x * (-sin_y)        + y * (sin_x * cos_y)                         + z * (cos_x * cos_y)                         + from.z;
}

void Mesh::rotate_all_axis(float angle_x, float angle_y, float angle_z, const std::vector<Point3 *>& point_list)
{
    Point3 mid = get_mid(point_list);

    //// Modulo to simplify calculus
    angle_x -= static_cast<int>(angle_x / (2.0f * PI)) * (2.0f * PI);
    angle_y -= static_cast<int>(angle_y / (2.0f * PI)) * (2.0f * PI);
    angle_z -= static_cast<int>(angle_z / (2.0f * PI)) * (2.0f * PI);

    for (auto point : point_list)
        rotate_point_all_axis(angle_x, angle_y, angle_z, point, mid);

    for (auto face : faces)
        face->update_normal();
}

void Mesh::rotate_all_axis(float angle_x, float angle_y, float angle_z)
{
    rotate_all_axis(angle_x, angle_y, angle_z, points);
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

    update_hit_box();
}

void Mesh::extrude_along_normal(float thickness, Triangle *face)
{
    auto *a = new Point3(*face->a + face->normal_ * thickness);
    auto *b = new Point3(*face->b + face->normal_ * thickness);
    auto *c = new Point3(*face->c + face->normal_ * thickness);

    extrude_face(face, a, b, c);
}

void Mesh::extrude_along_normal(float thickness, const std::vector<Triangle *>& faces_)
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

void add_normal(std::vector<Vector3> *list, const Vector3& normal)
{
    for (auto n : *list)
        if (n == normal)
            return;

    list->push_back(normal);
}

Vector3* Mesh::get_point_normal(const Point3 *point)
{
    std::vector<Triangle *> connected = get_faces(point);
    auto *normal = new Vector3(0, 0, 0);
    std::vector<Vector3> connected_normals;

    for (const auto face : connected)
        add_normal(&connected_normals, face->normal_);

    // std::cout << connected_normals.at(0) << std::endl;

    for (const auto& n : connected_normals)
        *normal += n;
    
    normal->normalize();
    return normal;
}

void Mesh::extrude_along_points_normalized(float thickness, Triangle *face)
{
    auto *a = new Point3(*face->a + *get_point_normal(face->a) * thickness);
    auto *b = new Point3(*face->b + *get_point_normal(face->b) * thickness);
    auto *c = new Point3(*face->c + *get_point_normal(face->c) * thickness);

    extrude_face(face, a, b, c);
}

void Mesh::extrude_along_points(float thickness, Triangle *face)
{
    Vector3 *normal_a = get_point_normal(face->a);
    Vector3 *normal_b = get_point_normal(face->b);
    Vector3 *normal_c = get_point_normal(face->c);

    float dot_a = dot(face->normal_, *normal_a);
    float dot_b = dot(face->normal_, *normal_b);
    float dot_c = dot(face->normal_, *normal_c);

    *normal_a /= dot_a; 
    *normal_b /= dot_b; 
    *normal_c /= dot_c; 

    auto *a = new Point3(*face->a + *normal_a * thickness);
    auto *b = new Point3(*face->b + *normal_b * thickness);
    auto *c = new Point3(*face->c + *normal_c * thickness);

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

std::vector<Point3 *> Mesh::get_points_from_faces(const std::vector<Triangle *>& faces_)
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

std::vector<Point3 *> get_points_from_edges(const std::vector<Edge>& edges)
{
    std::vector<Point3 *> point_list;

    for (auto edge : edges)
    {
        add_point_(&point_list, edge.a);
        add_point_(&point_list, edge.b);
    }

    return point_list;
}

std::vector<Vector3 *> Mesh::get_points_normal(const std::vector<Point3 *>& point_list)
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

std::vector<Edge> Mesh::get_edges(const std::vector<Triangle *>& faces_, std::vector<int> *count)
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
std::vector<Edge> Mesh::get_border_edges(const std::vector<Triangle *>& faces_)
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

void Mesh::extrude_along_points_normalized(float thickness, const std::vector<Triangle *>& faces_)
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

Triangle* get_face_from_point(const std::vector<Triangle *>& faces, Point3* point)
{
    for (auto face : faces)
        if (face->a == point || face->b == point || face->c == point)
            return face;
        
    std::cerr << "Error: get_face_from_point : Cannot find point in given faces\n";
    return nullptr;
}

void Mesh::extrude_along_points(float thickness, const std::vector<Triangle *>& faces_)
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
        float dot_new_point = dot(face->normal_, *point_direction.at(i));
        *point_direction.at(i) /= abs_(dot_new_point);

        // Shift the new point along its normal depending on the dot and the thickness
        auto *new_point = new Point3(*border_points.at(i) + *point_direction.at(i) * thickness);

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
        float dot_new_point = dot(face->normal_, *point_normal);
        *point_normal /= abs_(dot_new_point); // Absolute value of the dot, the normal should always be positive

        // Update the point position by shifting it along its normal depending on the dot and the thickness
        *point += *point_normal * thickness;
    }

    // Update faces' normal
    for (auto face : faces_)
        face->update_normal();
    update_hit_box();
}
