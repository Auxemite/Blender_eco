#pragma once

#include <vector>
#include "camera.hh"
#include "mesh.hh"
#include "light.hh"
#include "modifier.hh"

class Scene : NonMovable {
public:
    Camera camera;

    std::vector<Mesh *> meshes;
    std::vector<bool *> selectedMeshes;
    std::vector<Light *> lights;
    std::vector<Material *> materials;

    Mesh *xArrow = new Mesh("../data/xArrow.obj");
    Mesh *yArrow = new Mesh("../data/yArrow.obj");
    Mesh *zArrow = new Mesh("../data/zArrow.obj");

    glm::vec3 sceneCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    Modifier modifier;

    // Flags
    bool activate_grid = true;
    bool editmode = false;

    Scene()=default;
    void addMesh(const std::string& filename);
};