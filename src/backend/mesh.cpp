#include <strstream>
#include <fstream>
#include <filesystem>
#include "mesh.hh"

#define LINE_LEN 512
namespace fs = std::filesystem;

Mesh::Mesh(const std::string &filename) {
    // Open file as stream
    auto path = fs::current_path();
    std::ifstream f(filename);
    if (!f.is_open()) {
        std::cout << "Error File " << filename << " could not be opened";
        return;
    }

    glm::vec3 p;
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
            points.push_back(new glm::vec3(p));
        }

        // if first char is 'f' create associated face
        if (skip == 'f')
        {
            // Handle different format such as "f 3 6 9" and "f 3/4/5 6/7/8 9/10/11"
            int f[3];
            for (int & i : f)
            {
                s >> i;
                while (s.peek() != ' ' && !s.eof())
                    s >> skip;
            }

            // point index -1 because starting index is 1 in the .obj file
            faces.push_back(new Triangle(points[f[0] - 1],
                                         points[f[1] - 1],
                                         points[f[2] - 1]));
        }
    }

    material = nullptr;
    selected = false;
    is_visible = true;
}