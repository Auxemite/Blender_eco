#include <strstream>
#include <fstream>
#include <filesystem>
#include "mesh.hh"
#include "utils.hh"
#include <random>

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
            faces.push_back(new Triangle(f[0] - 1,
                                         f[1] - 1,
                                         f[2] - 1));
        }
    }

    selected = false;
    is_visible = true;
    graphicsObject = new GraphicsObject(vertices(), indices());
    this->update();
}

Mesh::Mesh(const Mesh& mesh) {
    faces.reserve(mesh.faces.size());
    for (auto face : mesh.faces) {
        faces.push_back(new Triangle(face->ia, face->ib, face->ic));
    }
    points.reserve(mesh.points.size());
    for (auto point : mesh.points) {
        points.push_back(new glm::vec3(*point));
    }

    material = mesh.material;
    midPoint = mesh.midPoint;
    selected = mesh.selected;
    is_visible = mesh.is_visible;
    graphicsObject = new GraphicsObject(vertices(), indices());
}

Mesh::~Mesh() {
    if (!graphicsObject)
        std::cerr << "Mesh Destructor Warning : Mesh had no graphicsObject\n";
    else
        delete graphicsObject;

    // materials are deleted in the scene object
    material = nullptr;

    for (auto face : faces) {
        delete face;
    }
    faces.clear();
    for (auto point : points) {
        delete point;
    }
    points.clear();
}

std::vector<Engine::vertex> Mesh::vertices() {
    auto vertices = std::vector<Engine::vertex>();
    for (auto point : points) {
        glm::vec3 color = material == nullptr ? glm::vec3(0.0) : material->color;
        struct Engine::vertex vertex = {
                *point,
                glm::vec3(),
                glm::vec2(point->x, point->y),
                glm::vec4(),
                color
        };
        vertices.push_back(vertex);
    }
    return vertices;
}

std::vector<Engine::vertex> Mesh::verticesEditmode() {
    auto vertices = std::vector<Engine::vertex>();
    for (int i = 0; i < points.size(); i++) {
        glm::vec3 color = glm::vec3(0.0);
        if (selectedPoints.contains(i))
            color = glm::vec3(1.0, 1.0, 0.0);
        glm::vec3 *point = points[i];
        struct Engine::vertex vertex = {
                *point,
                glm::vec3(),
                glm::vec2(point->x, point->y),
                glm::vec4(),
                color
        };
        vertices.push_back(vertex);
    }
    return vertices;
}

std::vector<u32> Mesh::indices() {
    auto indices = std::vector<u32>();
    for (auto & triangle : faces) {
        indices.push_back(triangle->ia);
        indices.push_back(triangle->ib);
        indices.push_back(triangle->ic);
    }
    return indices;
}

float Mesh::rayIntersection(const glm::vec3& cameraPos, const glm::vec3& direction) {
    float closestHitDistance = 0;
    for (auto & face : faces) {
        float hitScalar = face->rayIntersection(points, cameraPos, direction);
        float hitDistance = glm::length(direction * hitScalar);
        if (hitDistance > 0 && (closestHitDistance == 0 || closestHitDistance > hitDistance))
            closestHitDistance = hitDistance;
    }
    return closestHitDistance;
}

void Mesh::update() {
    // Update Mid Point
    glm::vec3 mid(0.0f, 0.0f, 0.0f);
    size_t nbPoint = points.size();
    for (auto point : points) {
        mid += *point;
    }
    midPoint = mid / static_cast<float>(nbPoint);
}

void Mesh::applyAndUpdate(const Modifier& modifier) {
    glm::mat3 rotationMat = getRotationMatrix(modifier.rotation);
    for (auto point : points) {
        *point = rotationMat * (*point - midPoint) * modifier.scale
        + modifier.position + midPoint;
    }
    graphicsObject->updateVBOFromMesh(vertices());
    update();
}

void Mesh::applySelectedAndUpdate(const Modifier& modifier) {
    glm::mat3 rotationMat = getRotationMatrix(modifier.rotation);
    for (auto selectedIndex : selectedPoints) {
        glm::vec3 *point = points[selectedIndex];
        *point = rotationMat * (*point - midPoint) * modifier.scale
                 + modifier.position + midPoint;
    }
    graphicsObject->updateVBOFromMesh(vertices());
    update();
}
