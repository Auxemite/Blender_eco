#pragma once

#include <vector>
#include "camera.hh"
#include "mesh.hh"
#include "light.hh"
#include "modifier.hh"

enum EditmodeType {
    VERTEX = 1,
    EDGE = 2,
    FACE = 0,
};

class Scene : NonMovable {
public:
    Camera camera;

    std::vector<Mesh *> meshes;
    std::vector<Light *> lights;
    std::vector<Material *> materials;

    std::vector<Mesh *> selectedMeshes;
//    Mesh *xArrow = new Mesh("../data/xArrow.obj");
//    Mesh *yArrow = new Mesh("../data/yArrow.obj");
//    Mesh *zArrow = new Mesh("../data/zArrow.obj");

    glm::vec3 sceneCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    Modifier modifier;

    // Flags
    bool editmode = false;
    EditmodeType editmodeType = FACE;
    bool shiftMode = false;

    Scene()=default;
    ~Scene();

    void drawSelectedMeshes(unsigned int shaderProgram, glm::vec3 unicolor);
    void drawMeshes(unsigned int shaderProgram, glm::vec3 unicolor);
    void drawOutline(unsigned int shaderProgram);

    void addMesh(const std::string& filename);
    void deleteMesh(int meshIndex);
    void duplicateMesh(int meshIndex);

    void clearSelectedMeshList();
    void toggleEditmode();
};