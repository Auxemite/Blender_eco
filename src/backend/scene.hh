#pragma once

#include <vector>
#include "camera.hh"
#include "mesh.hh"
#include "light.hh"
#include "modifier.hh"

class Scene {
public:
    Camera camera;

    std::vector<Mesh *> meshes;
    std::vector<Light *> lights;
    std::vector<Material *> materials;

    std::vector<int> selectedMeshes;
    glm::vec3 sceneCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    Modifier modifier;

    // Flags
    bool activate_grid = true;
    bool editmode = false;

    Scene()=default;
};