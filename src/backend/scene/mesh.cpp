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
    std::ifstream f(filename);
    if (!f.is_open())
        return;

    Point3 p;
    char skip;
    
    while (!f.eof())
    {

        char line[LINE_LEN];
        f.getline(line, LINE_LEN);

        std::strstream s;
        s << line;

        s >> skip;

        if (skip == 'v')
        {
            s >> p.x >> p.y >> p.z;
            points.push_back(new Point3(p));
        }

        if (skip == 'f')
        {
            int f[3];
            s >> f[0] >> f[1] >> f[2];
            faces.push_back(new Triangle(*points[f[0] - 1],
                                         *points[f[1] - 1],
                                         *points[f[2] - 1],
                                         uniformMaterial_));
        }
    }

    texture = uniformMaterial_;
}

double Mesh::ray_intersection(const Point3& cam_position, const Vector3& direction)
{
    if (faces.size() == 0)
        return -1.;
    
    double min_dist = -1.;
    for (auto iter = faces.begin(); iter < faces.end(); iter++)
    {
        Triangle *face = *iter;
        double face_dist = face->ray_intersection(cam_position, direction);

        if (face_dist == -1.)
            continue;
        
        if (min_dist == -1.)
            min_dist = face_dist;
        else
            min_dist = std::min(min_dist, face_dist);
    }

    return min_dist;
}

Vector3 Mesh::normal(const Point3& point) const { return faces.at(0)->normal_; }
Material Mesh::get_material(const Point3& point) const { return faces.at(0)->get_material(point); }
Shape_data Mesh::get_obj_data() const
{
    Shape_data test = faces.at(0)->get_obj_data();
    return test;
};

void Mesh::to_dot_obj(std::string filename)
{
    return;
}