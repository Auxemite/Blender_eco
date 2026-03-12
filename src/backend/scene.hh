#pragma once

#include <vector>
#include "camera.hh"
#include "mesh.hh"
#include "light.hh"
#include "modifier.hh"

struct indexMeshPoint {
    int meshIndex;
    int pointIndex;
};

enum EditmodeType {
    FACE,
    EDGE,
    VERTEX,

    NO_EDITMODE
};

class Scene : NonMovable {
public:
    Camera camera;

    std::vector<Mesh *> meshes;
    std::vector<Light *> lights;
    std::vector<Material *> materials;

    std::vector<Mesh *> selectedMeshes;
    std::vector<indexMeshPoint> selectedPoints;
//    Mesh *xArrow = new Mesh("../data/xArrow.obj");
//    Mesh *yArrow = new Mesh("../data/yArrow.obj");
//    Mesh *zArrow = new Mesh("../data/zArrow.obj");

    glm::vec3 sceneCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    Modifier modifier;

    // Flags
    bool editmode = false;
    EditmodeType editmodeType = NO_EDITMODE;

    Scene()=default;
    ~Scene();
    void addMesh(const std::string& filename);
    void deleteMesh(int meshIndex);
    void duplicateMesh(int meshIndex);

    void toggleEditmode(bool _editmode, EditmodeType _editmodeType);
};